//
//  ModuleManager.h
//  ORAGE
//
//  Created by Vincent Evrard on 9/11/21.
//

#ifndef ModuleManager_h
#define ModuleManager_h

#include "cables.h"
#include "Components.h"
#include "OrageFileTools.h"

namespace ORAGE {
    namespace COMMON {
        using namespace std;
        using namespace ORAGE::COMPONENTS;
        using namespace ORAGE::CONNECTIONS;
        
        class ModuleManager {
            vector<ModuleRef> modules;
            CablesRef cables;
            ci::JsonTree jCablesToCreate;
            std::vector<std::string>filesToOpen;
            typedef shared_ptr<ModuleManager> ModuleManagerRef;
            
            
            void openFiles(){
                if(filesToOpen.size() == 0 )return;
                auto it = filesToOpen.begin();
                while(it != filesToOpen.end()){
                    openRageFile((*it), [this](fs::path filePath){
                        if(filePath.filename().string() == "cables.json"){
                            addCables(filePath);
                        }else{
                            add(filePath);
                        }
                    });
                    filesToOpen.erase(it);
                }
            }
            
            void removeModules(){
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
                
            }
            
            bool updateModule(){
                bool allReady = true;
                for(auto module : modules){
                    module->update();
                    allReady &= module->isReady();
                }
                return allReady;
            }
            
            void addCables(){
                if(jCablesToCreate.hasChild("CABLES")){
                    for(auto jCable : jCablesToCreate.getChild("CABLES")){
                        if(jCable.hasChildren()){
                            for(int i = 0 ; i < jCable.getChildren().size() ; i++){
                                auto jPlug = jCable.getChild(i);
                                if(jPlug.hasChild("MODULE_ID")){
                                    auto jModule = jPlug.getChild("MODULE_ID");
                                    auto jAttr = jPlug.getChild("ATTR_NAME");
                                    auto module = getById(jModule.getValue<std::string>());
                                    if(!!module){
                                        auto attr = module->attributes()->get(jAttr.getValue<std::string>());
                                        module->UI->getParameter(attr->name())->eventTrigger({
                                            "plug", attr
                                        });
                                    }
                                }
                            }
                        }
                    }
                    jCablesToCreate.clear();
                }
            }
            
        public :
            ModuleManager(){
                cables = Cables::create();
            }
            static ModuleManagerRef create(){
                return std::make_shared<ModuleManager>();
            }
            
            ModuleRef getById(std::string id){
                for(auto module : modules){
                    if(module->getId() == id)return module;
                }
                return nullptr;
            }
            
            void empty(){
                auto it = modules.begin();
                while(it != modules.end()){
                    for(auto attr : (*it)->attributes()->every()){
                        cables->removeCablesPlugTo(attr);
                    }
                    modules.erase(it);
                }
            }
            
            void addFileToOpen(std::vector<std::string> filesToOpen){
                this->filesToOpen.insert( this->filesToOpen.end(), filesToOpen.begin(), filesToOpen.end() );
            }
            
            void addCables(fs::path filePath){
                ifstream fin;
                fin.open(filePath);
                if (fin.is_open())    {
                    jCablesToCreate = ci::JsonTree(static_cast<stringstream const &>(stringstream() << fin.rdbuf()).str() );
                }
                fin.close();
            }
            
            ModuleRef add(fs::path filePath){
                string _name = "";
                string _ext = "";
                TYPES currentType = pathToComponentType(filePath, &_name, &_ext);
                return add(filePath, vec2(0), currentType);
            }
                
            ModuleRef add(fs::path filePath, ci::vec2 pos, TYPES type){
                string name = "";
                string ext = "";
                splitNameExtension(filePath.filename().generic_string(), type, &name, &ext);
                
                ModuleRef module;
                switch(type){
                    case TYPES::ISF :
                    case TYPES::FX :
                    case TYPES::OUTPUT :
                        module = ModuleISF::create(name, filePath.string(), type);
                        
                    break;
                    case TYPES::CONTROLLER :
                    case TYPES::MATH :
                        module = ModuleController::create(name, filePath.string(), type);
                        
                    break;
                        
                    case TYPES::CLOCK :
//                        if(name == "source"){
//                            module = ModuleOscServer::create(name, type);
//                        }else{
                            module = ModuleController::create(name, filePath.string(), type);
                        
//                        }
                        break;
                    case TYPES::INPUT :
                        if(name == "SyphonSpout"){
                            module = ModuleSyphonSpout::create(name, type);
                        }else if(name == "UI"){
                            module = ModuleUI::create(name, type);
                        }else if(name == "SORT"){
                            module = ModuleSort::create(name, type);
                        }
                    break;
                    case TYPES::OSC :
                        if(name == "server"){
                            module = ModuleOscServer::create(name, type);
                        }
                        if(name == "receiver"){
                            module = ModuleOscDataIn::create(name, type);
                        }
                        if(name == "filter"){
                            module = ModuleOscFilter::create(name, type);
                        }
                        if(name == "address"){
                            module = ModuleOscAddress::create(name, type);
                        }
                        if(name == "sender"){
                            module = ModuleOscDataOut::create(name, type);
                        }
                    break;
                    default :
                        return nullptr;
                    break;
                }
                module->setOrigin(pos == vec2(0) ? module->getOrigin(true) : pos);
                
                module->onParameterPlug([this](Evt evt){
                    if(evt.is("plug")){
                        cables->addCable(evt.target);
                    }
                    if(evt.is("rmplug")){
                        cables->removeCablesPlugTo(evt.target);
                    }
                });
                
                module->addEventListener([this](EvtModule evt){
                    if(evt.is("putAtTop")){
                        auto it = std::find(modules.begin(), modules.end(), evt.target);
                        if(it != modules.end() && it != modules.end() - 1){
                            std::rotate(it, it + 1, modules.end());
                        }
                    }
                });
                modules.push_back(module);
                return module;
            }
            
            void update(){
                openFiles();
                removeModules();
                bool allReady = updateModule();
                if(allReady){
                    addCables();
                }
            }
            
            void draw(){
                for(auto module : modules){
                    module->draw();
                }
                cables->draw();
            }
            
            void savePatch(){
                saveRageFile([&](fs::path tempPath){
                    for(auto module : modules){
                        string fileName = module->name(true) + componentTypeToFileExtention(module->moduleType);
                        saveFile(tempPath / fileName, module->serialize());
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
                                    plug.addChild(ci::JsonTree("MODULE_ID", module->getId()));
                                    plug.addChild(ci::JsonTree("ATTR_NAME", attr->name()));
                                    cable.addChild(plug);
                                }
                            }
                        }
                        jCables.addChild(cable);
                    }
                    tree.addChild(jCables);
                    saveFile(tempPath / "cables.json", tree.serialize());
                });
            }
        };//ModuleManager
        typedef shared_ptr<ModuleManager> ModuleManagerRef;
    }
}

#endif /* ModuleManager_h */
