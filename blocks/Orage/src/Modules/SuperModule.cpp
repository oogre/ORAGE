//
//  SuperModule.cpp
//  PianoProjete
//
//  Created by Vincent Evrard on 2017-05-17.
//
//

#include "SuperModule.h"

namespace ogre {
    
    SuperModule::SuperModule():ModuleCommon(to_string(id), vec2(0, 0), vec2(200, 200)){
    
    }
    
    void SuperModule::addModule(ModuleRef module, const std::function<void( )> &callback){
        modules.push_back(module);
        callback();
    }
    
    void SuperModule::clear(const std::function<void( ModuleRef )> &callback){
        auto it = modules.begin();
        auto end = modules.end();
        for(; it != end ; it ++){
            callback(*it);
        }
        modules.clear();
    }
    
    void SuperModule::update(){
        auto it = modules.begin();
        for(; it != modules.end() ; ){
            if((*it)->shouldDestroy){
                (*it).reset();
                it = modules.erase(it);
            }else{
                //cout<<(*it)->modules.size()<<endl;
                /*
                    auto it2 = (*it)->modules.begin();
                for(; it2 != groupedModules.end() ; it2 ++){
                    (*it2)->update();
                    }
                    */
                it ++;
            }
        }
    }
}
