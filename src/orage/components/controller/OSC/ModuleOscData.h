//
//  ModuleOscData.h
//  ORAGE
//
//  Created by Vincent Evrard on 7/12/21.
//

#ifndef ModuleOscData_h
#define ModuleOscData_h
#include "Module.h"
namespace ORAGE {
    namespace COMPONENTS {
        using namespace ci;
        using namespace ci::gl;
        using namespace ci::app;
        using namespace std;
        using namespace ISF;
        
        class ModuleOscData : public Module{
            typedef shared_ptr<ModuleOscData> ModuleOscDataRef;
            
            ModuleOscData(string name, TYPES type) :
                Module(name)
            {
                moduleType = type;
            }
            
        protected:
        public :
            virtual ~ModuleOscData(){
               
            }
            static ModuleOscDataRef create(string name, TYPES type = TYPES::OSC){
                return ModuleOscDataRef(new ModuleOscData(name, type));
            }
        };//ModuleOscData
        typedef shared_ptr<ModuleOscData> ModuleOscDataRef;
    }//COMPONENTS
}//ORAGE

#endif /* ModuleOscData_h */
