//
//  Cables.h
//  Orage
//
//  Created by Vincent Evrard on 8/07/21.
//

#ifndef Cables_h
#define Cables_h

#include "Cable.h"

namespace ORAGE {
    namespace CORE {
        using namespace std;
        using namespace ci;
        using namespace ci::gl;
        
        class Cables : public COMMON::EventTemplate<Cable> {
            typedef shared_ptr<class Cables> CablesRef;
            typedef map<string, CableRef> CablesWrapper;
            typedef COMMON::EventTemplate<Cable> CableEvt;
            CablesWrapper cables;
        protected :
            Cables():
                CableEvt()
            {
            }
        public :
            static CablesRef create(){
                return CablesRef( new Cables() );
            }
            virtual ~Cables(){
                
            }
            void addCable(ModuleRef input, ModuleRef output, bool trigEvent = true){
                CableRef cable = Cable::create(input, output);
                cables[output->getName(true)+">>>"+input->getName(true)] = cable;
                if(trigEvent){
                    CableEvt::eventTrigger({"add", cable});
                }
            }
        };//class Cables
        typedef std::shared_ptr<class Cables> CablesRef;
    }//namespace CORE {
}//namespace ORAGE {

#endif /* Cables_h */
