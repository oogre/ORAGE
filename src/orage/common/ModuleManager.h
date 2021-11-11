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
#include "cables.h"
#include "ModuleTypes.h"

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
                    default :
                        return;
                    break;
                }
                module->setOrigin(pos + vec2(0, 25));
                module->addEventListenerOnParameters([&](Evt evt){
                    if(evt.is("plug")){
                        cables->addCable(evt.target);
                    }
                });
                module->addEventListener([&](EvtModule evt){
                    if(evt.is("putAtTop")){
                        auto it = std::find(modules.begin(), modules.end(), evt.target);
                        if(it != modules.end() && it != modules.end() - 1){
                            std::rotate(it, it + 1, modules.end());
                        }
                    }
                });
                modules.push_back(module);
            }
            
            void update(){
                auto it = modules.begin();
                while(it != modules.end()){
                    if((*it)->hasToDestroy()){
                        for(auto [key, param] : (*it)->UI->parameters){
                            cables->removeCablesPlugTo(param);
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
            
        };//ModuleManager
        typedef shared_ptr<ModuleManager> ModuleManagerRef;
    }
}

#endif /* ModuleManager_h */
