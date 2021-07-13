//
//  Orage.h
//  Orage
//
//  Created by Vincent Evrard on 29/06/21.
//

#ifndef Orage_h
#define Orage_h
#include "Number.h"
#include "Clock.h"
#include "Cables.h"

namespace ORAGE {
    namespace CORE {
        using namespace std;
        
        enum class ModuleType {
            NumberI,
            NumberF,
            Clock,
            Module,
        };
        
        class Manager {
            Manager(){
                module = Module::create("CORE");
                cables = Cables::create();
            }
            Manager(const Manager &old);
            const Manager &operator=(const Manager &old);
        public:
            ModuleRef module;
            CablesRef cables;
            
            static Manager &Instance(){
                static auto_ptr<Manager> instance( new Manager );
                return *instance;
            }
            virtual ~Manager(){
                
            }
            void addCables(json cables){
                for (const auto& item : cables.items()){
                    addCable(item.value());
                }
            }
            void addCable(json cable){
                string inputName = cable[1] ;
                string outputName = cable[0] ;
                ModuleRef input = getModule(inputName);
                ModuleRef output = getModule(outputName);
                cables->addCable(input, output);
            }
            
            void addModules(json modules){
                for (const auto& item : modules.items()){
                    addModule(item.value());
                }
            }   
            void addModule(json conf){
                if(!conf.contains("/type"_json_pointer) || !conf.contains("/name"_json_pointer)){
                    throw std::invalid_argument( "ORAGE::CORE::Manager::addModule need /type /name");
                }
                ModuleType type = conf.at("/type"_json_pointer);
                string name = conf.at("/name"_json_pointer);
                
                ModuleRef newModule;
                switch(type){
                    case ModuleType::NumberF : {
                        newModule = NumberF::create(name);
                        break;
                    }
                    case ModuleType::NumberI : {
                        newModule = NumberI::create(name);
                        break;
                    }
                    case ModuleType::Clock : {
                        newModule = Clock::create(name);
                        break;
                    }
                    default : {
                        newModule = Module::create(name);
                        break;
                    }
                }
                newModule->setConf(conf);
                module->addModule( newModule );
            }
            
            template<typename T = Module>
            shared_ptr<T> getModule(string name){
                return module->getModule(name)->as<T>();
            }
            string to_string(){
                return module->to_string();
            }
        };//class Manager {
    }//namespace CORE {
}//namespace ORAGE {
#endif /* Orage_h */
