//
//  OrageFileTools.h
//  ORAGE
//
//  Created by Vincent Evrard on 1/12/21.
//

#ifndef OrageFileTools_h
#define OrageFileTools_h

#include <zip.h>
#include <iostream>
#include <string>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include "cinder/app/Platform.h"
#if defined(CINDER_MSW)
    #include <winreg.h>
    #include <winerror.h>
    #include "cinder/app/App.h"
    #include <atlstr.h>
    #include <shellapi.h>
#endif


#define PRINT(arg) #arg
#define XPRINT(s) PRINT(s)


namespace ORAGE {
    namespace COMMON {
        static bool is_dir(const std::string& dir)
        {
            struct stat st;
            ::stat(dir.c_str(), &st);
            return S_ISDIR(st.st_mode);
        }

        static void walk_directory(const std::string& startdir, const std::string& inputdir, zip_t *zipper)
        {
            DIR *dp = ::opendir(inputdir.c_str());
            if (dp == nullptr) {
                throw std::runtime_error("Failed to open input directory: " + std::string(::strerror(errno)));
            }
            
            struct dirent *dirp;
            while ((dirp = readdir(dp)) != NULL) {
                if (dirp->d_name != std::string(".") && dirp->d_name != std::string("..")) {
                    std::string fullname = inputdir + "/" + dirp->d_name;
                    if (is_dir(fullname)) {
                        if (zip_dir_add(zipper, fullname.substr(startdir.length() + 1).c_str(), ZIP_FL_ENC_UTF_8) < 0) {
                            throw std::runtime_error("Failed to add directory to zip: " + std::string(zip_strerror(zipper)));
                        }
                        walk_directory(startdir, fullname, zipper);
                    } else {
                        zip_source_t *source = zip_source_file(zipper, fullname.c_str(), 0, 0);
                        if (source == nullptr) {
                            throw std::runtime_error("Failed to add file to zip: " + std::string(zip_strerror(zipper)));
                        }
                        if (zip_file_add(zipper, fullname.substr(startdir.length() + 1).c_str(), source, ZIP_FL_ENC_UTF_8) < 0) {
                            zip_source_free(source);
                            throw std::runtime_error("Failed to add file to zip: " + std::string(zip_strerror(zipper)));
                        }
                    }
                }
            }
            ::closedir(dp);
        }
        
        static void zip_directory(const std::string& inputdir, const std::string& output_filename)
        {
            int errorp;
            zip_t *zipper = zip_open(output_filename.c_str(), ZIP_CREATE | ZIP_EXCL, &errorp);
            if (zipper == nullptr) {
                zip_error_t ziperror;
                zip_error_init_with_code(&ziperror, errorp);
                throw std::runtime_error("Failed to open output file " + output_filename + ": " + zip_error_strerror(&ziperror));
            }
            
            try {
                walk_directory(inputdir, inputdir, zipper);
            } catch(...) {
                zip_close(zipper);
                throw;
            }
            
            zip_close(zipper);
        }
        
        static void saveFile(ci::fs::path path, std::string content){
            std::ofstream oStream(path.generic_string());
            oStream << content;
            oStream.close();
        }
        
        static void rmDir(ci::fs::path path){
            if(!ci::fs::exists( path )) return;
            if (ci::fs::is_directory(path)){
                for (auto & child : ci::fs::directory_iterator(path)){
                    rmDir(child.path());
                }
            }
            ci::fs::remove(path);
        };
        
        static std::string toHex(std::string s){
            std::stringstream ss;
            for(int i = 0; i < s.size(); i++) {
                int character = int(s[i]); // converting each character to its ascii value
                ss << std::hex << character; // basefield is now set to hex
            }
            return ss.str();
        }
        
        std::string replaceAll(std::string str, const std::string& from, const std::string& to) {
            size_t start_pos = 0;
            while((start_pos = str.find(from, start_pos)) != std::string::npos) {
                str.replace(start_pos, from.length(), to);
                start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
            }
            return str;
        }
        
        static void openWith(ci::fs::path filePath){
        #if defined(CINDER_MAC)
            std::string appPath = ci::app::Platform::get()->getExecutablePath().generic_string()+"/"+XPRINT(PRODUCT_NAME)+".app";
            std::string cmd = "xattr -wx com.apple.LaunchServices.OpenWith 62706C6973743030D30102030405065776657273696F6E54706174685F101062756E646C656964656E74696669657210005F1038[APP_PATH]5F1014[APP_ID]080F171C2F316C0000000000000101000000000000000700000000000000000000000000000083 " + filePath.generic_string();
            cmd = replaceAll(cmd, "[APP_PATH]", toHex(appPath));
            cmd = replaceAll(cmd, "[APP_ID]", toHex(XPRINT(PRODUCT_BUNDLE_IDENTIFIER)));
            std::cout << "on dbClick run : " << appPath << std::endl;
            system(&cmd[0]);
        #elif defined(CINDER_MSW)
            HKEY hkey;
            CString sExt = ".rage";
            CString sDescription = "FileExtensionForOrage";
            CString app((ci::app::Platform::get()->getExecutablePath().generic_string() + XPRINT(PRODUCT_NAME) + ".exe").c_str());
            if (ERROR_SUCCESS == RegOpenKey(HKEY_CLASSES_ROOT, sExt, &hkey)){
                RegCloseKey(hkey);
                std::cout << "This Extension is already registered" << std::endl;
                return;
            }

           auto s2ws = [](const std::string& s) {
                int len;
                int slength = (int)s.length() + 1;
                len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
                wchar_t* buf = new wchar_t[len];
                MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
                std::wstring r(buf);
                delete[] buf;
                return r;
            };

            ShellExecute(NULL, s2ws("runas").c_str(), s2ws(ci::app::getAssetPath("icns/run.bat").generic_string()).c_str(), 0, 0, SW_SHOWNORMAL);
;
        #endif
        }
        
        static void setIconTo(ci::fs::path filePath){
        #if defined(CINDER_MAC)
            // /usr/bin/DeRez -only icns yellow.rage > ci::app::getAssetPath("icns/icns.rsrc").generic_string()
            std::string cmd = "/usr/bin/Rez -append " + ci::app::getAssetPath("icns/icns.rsrc").generic_string() + " -o " + filePath.generic_string() + "; /usr/bin/SetFile -a C "+ filePath.generic_string() + " ; ";
            system(&cmd[0]);
        #elif defined(CINDER_MSW)

        #endif
        }
        
        static zip * get_archive(std::string path, int flags) {
            int error = 0;
            zip *archive = zip_open(path.c_str(), flags , &error);
            
            if(!archive) {
                std::cout << "could not open archive" << path << std::endl;
                exit(1) ;
            }
            std::cout << "Done: opening archieve" << path <<  std::endl;
            return archive;
        }
        
        static std::map<std::string, std::string> unzip_directory(const std::string& path) {
            std::map<std::string, std::string> files;
            zip *archive = get_archive(path, ZIP_RDONLY);
            int files_total = zip_get_num_entries(archive, 0);
            std::cout << "Note: we have " << files_total << " files in ZIP" << std::endl;
            for (int i = 0; i < zip_get_num_entries(archive, 0); i++) {
                struct zip_stat sb;
                int len;
                long long sum = 0;
                if (zip_stat_index(archive, i, 0, &sb) == 0) {
                    std::cout << "Name: " << sb.name;
                    std::cout << " Size: " << sb.size;
                    std::cout << " Index: " << sb.index;
                    std::cout << " Valid: " << sb.valid;
                    std::cout << " mtime: " << sb.mtime << std::endl;
                    zip_file *zf = zip_fopen_index(archive, i, 0);
                    if (!zf) {
                        std::cout << "failed to opne  entry of archive. " << zip_strerror(archive) << std::endl;
                        zip_close(archive);
                    }
                    sum = 0;
                    std::stringstream ss;
                    char buffer[10000];
                    while (sum != sb.size) {
                        memset(buffer, 0, sizeof buffer);
                        len = zip_fread(zf, buffer, 100);
                        if (len < 0) {
                            exit(102);
                        }
                        ss << buffer ;
                        sum += len;
                        
                    }
                    files[sb.name] = ss.str();
                    zip_fclose(zf);
                }
            }
            zip_close(archive);
            return files;
        }
        
        static void saveRageFile(std::function<void(ci::fs::path filePath)> callback){
            ci::fs::path defaultDir = ci::getDocumentsDirectory() / "ORAGE";
            if(!ci::fs::exists( defaultDir )){
                ci::fs::create_directories(defaultDir);
            }
            std::vector<std::string> fileExtension = std::vector<std::string>(1, "rage");
            ci::fs::path path = ci::app::getSaveFilePath(defaultDir, fileExtension);
            ci::fs::path tempPath = ci::fs::path(path.generic_string()+".temp");
            rmDir(path);
            rmDir(tempPath);
            ci::fs::create_directories(tempPath);
            callback(tempPath);
            zip_directory(tempPath.generic_string() , path.generic_string());
            openWith(path);
            setIconTo(path);
            rmDir(tempPath);
        }
        
        static void openRageFile(std::string fileName, std::function<void(ci::fs::path filePath)> callback){
            std::map<std::string, std::string> files = unzip_directory(fileName);
            ci::fs::path tmpPath = ci::fs::path(fileName+".tmp");
            rmDir(tmpPath);
            ci::fs::create_directories(tmpPath);
            for(auto [name, file] : files){
                saveFile(tmpPath / name, file);
                callback(tmpPath / name);
            }
            rmDir(tmpPath);
        }
    }//COMMON
}//ORAGE


#endif /* OrageFileTools_h */
