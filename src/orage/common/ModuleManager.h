//
//  ModuleManager.h
//  ORAGE
//
//  Created by Vincent Evrard on 9/11/21.
//

#ifndef ModuleManager_h
#define ModuleManager_h

#include "ModuleISF.h"
#include "ModuleController.h"
#include "ModuleSyphonSpout.h"
#include "cables.h"
#include "ModuleTypes.h"
#include <zip.h>

#include <iostream>
#include <string>

#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

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
namespace ORAGE {
    namespace COMMON {
        using namespace std;
        using namespace ORAGE::COMPONENTS;
        using namespace ORAGE::CONNECTIONS;
        
        class ModuleManager {
            vector<ModuleRef> modules;
            CablesRef cables;
            
            typedef shared_ptr<ModuleManager> ModuleManagerRef;
            ModuleManager(){
                cables = Cables::create();
            }
            
        public :
            static ModuleManagerRef create(){
                return ModuleManagerRef(new ModuleManager());
            }
            
            void add(fs::path filePath, ci::vec2 pos, TYPES type){
                string name = filePath.filename().string();
                string ext = filePath.extension().string();
                name = name.substr(0, name.length() - ext.length());
                ModuleRef module;
                switch(type){
                    case TYPES::ISF :
                    case TYPES::FX :
                    case TYPES::OUTPUT :
                        module = ModuleISF::create(name, filePath.string(), type);
                    break;
                    case TYPES::CONTROLLER :
                    case TYPES::CLOCK :
                    case TYPES::MATH :
                        module = ModuleController::create(name, filePath.string(), type);
                    break;
                    case TYPES::INPUT :
                        module = ModuleSyphonSpout::create(name, type);
                    break;
                    default :
                        return;
                    break;
                }
                module->setOrigin(pos + vec2(0, 25));
                
                module->addEventListener([&, module](EvtModule evt){
                    if(evt.is("putAtTop")){
                        auto it = std::find(modules.begin(), modules.end(), evt.target);
                        if(it != modules.end() && it != modules.end() - 1){
                            std::rotate(it, it + 1, modules.end());
                        }
                    }
                    if(evt.is("ready")){
                        module->addEventListenerOnParameters([&](Evt evt){
                            if(evt.is("plug")){
                                cables->addCable(evt.target);
                            }
                        });
                    }
                });
                modules.push_back(module);
            }
            
            void update(){
                auto it = modules.begin();
                while(it != modules.end()){
                    if((*it)->hasToDestroy()){
                        for(auto attr : (*it)->attributes()->every()){
                            cables->removeCablesPlugTo(attr);
                        }
                        modules.erase(it);
                        continue;
                    }
                    it++;
                }
                for(auto module : modules){
                    module->update();
                }
            }
            void draw(){
                for(auto module : modules){
                    module->draw();
                }
            }
            
            void save(string location){
                vector<string> fileExtension = vector<string>(1, "rage");
                fs::path path = getSaveFilePath(location, fileExtension);
                std::function<void(fs::path)> rmDir;
                rmDir = [&rmDir](fs::path path){
                    if (fs::is_directory(path)){
                        for (auto & child : fs::directory_iterator(path)){
                            rmDir(child.path());
                        }
                    }
                    fs::remove(path);
                };
                if(fs::exists( path )){
                    rmDir(path);
                }
                fs::path tempPath = fs::path(path.generic_string()+".temp");
                cout<<tempPath<<endl;
                if(fs::exists( tempPath )){
                    rmDir(tempPath);
                }
                fs::create_directories(tempPath);
                for(auto module : modules){
                    string fileName = module->name();
                    switch(module->moduleType){
                        case ISF :
                        case FX :
                        case OUTPUT :
                            fileName += ".fs";
                            break;
                        case CLOCK :
                        case CONTROLLER :
                        case MATH :
                            fileName += ".js";
                            break;
                        case INPUT :
                            break;
                        default :
                            break;
                    }
                    fs::path filePath = tempPath / fileName;
                    ofstream oStream(filePath.generic_string());
                    oStream << module->serialize();
                    oStream.close();
                }
                
                
                ci::JsonTree tree = ci::JsonTree();
                ci::JsonTree jCables = ci::JsonTree::makeArray("CABLES");
                std::vector<CablesID> cableIds = cables->getKeys();
                for(CablesID cableId : cableIds){
                    ci::JsonTree cable = ci::JsonTree::makeArray();
                    for(auto module : modules){
                        for(auto attr : module->attributes()->every()){
                            if(attr.get() == cableId.first || attr.get() == cableId.second){
                                ci::JsonTree plug = ci::JsonTree();
                                plug.addChild(ci::JsonTree("MODULE_NAME", module->name()));
                                plug.addChild(ci::JsonTree("ATTR_NAME", attr->name()));
                                cable.addChild(plug);
                            }
                        }
                    }
                    jCables.addChild(cable);
                }
                tree.addChild(jCables);
                fs::path filePath = tempPath / "cables.json";
                ofstream oStream(filePath.generic_string());
                oStream << tree.serialize();
                oStream.close();
                
                zip_directory(tempPath.generic_string() , path.generic_string());
                if(fs::exists( tempPath )){
                    rmDir(tempPath);
                }
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
                std::string cmd = "xattr -wx com.apple.LaunchServices.OpenWith 62706C6973743030D30102030405065776657273696F6E54706174685F101062756E646C656964656E74696669657210005F10382F55736572732F6F6772652F776F726B732F313230322F4F524147452F78636F64652F6275696C642F44656275672F4F524147452E6170705F10146F72672E6C696263696E6465722E4F5241474532080F171C2F316C0000000000000101000000000000000700000000000000000000000000000083 "+path.generic_string();
                system(&cmd[0]);
                
                // AND ICON
            }
        };//ModuleManager
        typedef shared_ptr<ModuleManager> ModuleManagerRef;
    }
}

#endif /* ModuleManager_h */
