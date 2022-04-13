//
//  ModuleClock.h
//  ORAGE
//
//  Created by Vincent Evrard on 11/12/21.
//

#ifndef ModuleClock_h
#define ModuleClock_h
#include "Module.h"
namespace ORAGE {
    namespace COMPONENTS {
        using namespace ci;
        using namespace ci::gl;
        using namespace ci::app;
        using namespace std;
        using namespace ISF;
        
        class ModuleClock : public Module {
            typedef shared_ptr<ModuleClock> ModuleClockRef;
            string rawContent;
            
        public :
            ModuleClock(string name, string path, TYPES type) :
            Module(name)
            {
                moduleType = type;
                rawContent = ORAGE::COMMON::readFile(path);
            }
//            virtual vec2 getOrigin(bool raw=false) override {
//                if(raw){
//                    try{
//                        string rawPosition = dukglue_pcall_method<string>( ctx, jsObject, "getPosition");
//                        ci::JsonTree origin = ci::JsonTree(rawPosition);
//                        if(origin.hasChild("x") && origin.hasChild("x")){
//                            return vec2(
//                                        origin.getChild("x").getValue<float>(),
//                                        origin.getChild("y").getValue<float>()
//                                        );
//                        }
//                    }catch(DukErrorException & e){
//                        onError(e);
//                    }
//                }
//                return Module::getOrigin(raw);
//            }
//            virtual string serialize() {
//                string r = "";
//                try{
//                    return dukglue_pcall_method<string>( ctx, jsObject, "rebuild", rawContent, Module::serialize() );
//                }catch(DukErrorException & e){
//                    onError(e);
//                }
//                return r;
//            }
            
            virtual ~ModuleClock(){
                //duk_destroy_heap(ctx);
                cout<<"~ModuleClock"<<endl;
            }
            static ModuleClockRef create(string name, string path, TYPES type = TYPES::CONTROLLER){
                return std::make_shared<ModuleClock>(name, path, type);
            }
        };
        typedef shared_ptr<ModuleClock> ModuleClockRef;
    }//COMPONENTS
}//ORAGE

#endif /* ModuleClock_h */
