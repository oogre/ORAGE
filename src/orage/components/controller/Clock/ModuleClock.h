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
            
            
            
        public :
            ModuleClock(string name, string path, TYPES type) :
            Module(name)
            {
                moduleType = type;
            }
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
