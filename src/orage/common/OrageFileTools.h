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
        
        static void saveFile(fs::path path, std::string content){
            ofstream oStream(path.generic_string());
            oStream << content;
            oStream.close();
        }
        
        static void rmDir(fs::path path){
            if(!fs::exists( path )) return;
            if (fs::is_directory(path)){
                for (auto & child : fs::directory_iterator(path)){
                    rmDir(child.path());
                }
            }
            fs::remove(path);
        };
        
        static void openWith(fs::path filePath, fs::path appPath = fs::path()){
            /*
             com.apple.LaunchServices.OpenWith:
             00000000  62 70 6C 69 73 74 30 30 D3 01 02 03 04 05 06 57  |bplist00.......W|
             00000010  76 65 72 73 69 6F 6E 54 70 61 74 68 5F 10 10 62  |versionTpath_..b|
             00000020  75 6E 64 6C 65 69 64 65 6E 74 69 66 69 65 72 10  |undleidentifier.|
             00000030  00 5F 10 38 2F 55 73 65 72 73 2F 6F 67 72 65 2F  |._.8/Users/ogre/|
             00000040  77 6F 72 6B 73 2F 31 32 30 32 2F 4F 52 41 47 45  |works/1202/ORAGE|
             00000050  2F 78 63 6F 64 65 2F 62 75 69 6C 64 2F 44 65 62  |/xcode/build/Deb|
             00000060  75 67 2F 4F 52 41 47 45 2E 61 70 70 5F 10 14 6F  |ug/ORAGE.app_..o|
             00000070  72 67 2E 6C 69 62 63 69 6E 64 65 72 2E 4F 52 41  |rg.libcinder.ORA|
             00000080  47 45 32 08 0F 17 1C 2F 31 6C 00 00 00 00 00 00  |GE2..../1l......|
             00000090  01 01 00 00 00 00 00 00 00 07 00 00 00 00 00 00  |................|
             000000A0  00 00 00 00 00 00 00 00 00 83                    |..........|
             000000aa
             */
            std::string cmd = "xattr -wx com.apple.LaunchServices.OpenWith 62706C6973743030D30102030405065776657273696F6E54706174685F101062756E646C656964656E74696669657210005F10382F55736572732F6F6772652F776F726B732F313230322F4F524147452F78636F64652F6275696C642F44656275672F4F524147452E6170705F10146F72672E6C696263696E6465722E4F5241474532080F171C2F316C0000000000000101000000000000000700000000000000000000000000000083 "+filePath.generic_string();
            system(&cmd[0]);
        }
        static zip * get_archive(string path, int flags) {
            int error = 0;
            zip *archive = zip_open(path.c_str(), flags , &error);
            
            if(!archive) {
                cout << "could not open archive" <<  path << endl;
                exit(1) ;
            }
            cout << "Done: opening archieve" << path <<  endl;
            return archive;
        }
        
        static std::map<std::string, std::string> unzip_directory(const std::string& path) {
            std::map<std::string, std::string> files;
            zip *archive = get_archive(path, ZIP_RDONLY);
            int files_total = zip_get_num_entries(archive, 0);
            cout << "Note: we have " << files_total << " files in ZIP" << endl;
            for (int i = 0; i < zip_get_num_entries(archive, 0); i++) {
                struct zip_stat sb;
                int len;
                long long sum = 0;
                if (zip_stat_index(archive, i, 0, &sb) == 0) {
                    cout << "Name: " << sb.name;
                    cout << " Size: " << sb.size;
                    cout << " Index: " << sb.index;
                    cout << " Valid: " << sb.valid;
                    cout << " mtime: " << sb.mtime << endl;
                    zip_file *zf = zip_fopen_index(archive, i, 0);
                    if (!zf) {
                        cout << "failed to opne  entry of archive. " << zip_strerror(archive) << endl;
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
    }//COMMON
}//ORAGE


#endif /* OrageFileTools_h */
