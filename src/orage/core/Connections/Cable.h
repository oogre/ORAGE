//
//  Cable.h
//  Orage
//
//  Created by Vincent Evrard on 22/06/21.
//

#ifndef Cable_h
#define Cable_h

namespace ORAGE {
    namespace CORE {
        using namespace std;
        
        class Cable {
        public:
            ModuleRef input;
            ModuleRef output;
        protected:
            typedef shared_ptr<class Cable> CableRef;
            typedef ORAGE::COMMON::Event<Module> ModuleEvt;
            
            Cable(ModuleRef input, ModuleRef output) :
                input(input),
                output(output)
            {
                this->output->addEventListener("change", [&](ModuleEvt event) -> void{
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
            string to_string(){
                return output->getName(true)+">>>"+input->getName(true);
            }
        };//class Cable
        typedef shared_ptr<class Cable> CableRef;
    }//namespace CORE
}//namespace ORAGE
#endif /* Cable_h */
