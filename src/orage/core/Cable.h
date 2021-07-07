//
//  Cable.h
//  Orage
//
//  Created by Vincent Evrard on 22/06/21.
//

#ifndef Cable_h
#define Cable_h

#include "Module.h"

namespace ORAGE {
    namespace CORE {
        using namespace std;
        //using namespace boost::signals2;
        
        class Cable {
            ModuleRef input;
            ModuleRef output;
        protected:
            typedef std::shared_ptr<class Cable> CableRef;
            Cable(ModuleRef input, ModuleRef output) :
                input(input),
                output(output)
            {
                this->output->addEventListener("change", [&](ORAGE::CORE::ParameterEvent event) -> void{
                    this->input->setValue(event.target->getValue());
                });
            }
        public :
            static CableRef create(ModuleRef input, ModuleRef output){
                return CableRef( new Cable( input, output ) );
            }
            virtual ~Cable(){
//                output->removeEventListener("change", [&](ORAGE::CORE::ParameterEvent event) -> void{
//                    input->setValue(event.target->getValue());
//                });
            }
        };//class Cable
        typedef shared_ptr<class Cable> CableRef;
    }//namespace CORE
}//namespace ORAGE
#endif /* Cable_h */
