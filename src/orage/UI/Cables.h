//
//  Cables.h
//  Orage
//
//  Created by Vincent Evrard on 8/07/21.
//

#ifndef Cables_ui_h
#define Cables_ui_h

#include "../common/EventTemplate.h"
#include "Cable.h"
#include "Plug.h"
namespace ORAGE {
    namespace UI {
        using namespace std;
        using namespace ci;
        using namespace ci::gl;
        
        class Cables : public COMMON::EventTemplate<Cable> {
            typedef shared_ptr<class Cables> CablesRef;
            typedef map<string, CableRef> CablesWrapper;
            CablesWrapper cables;
        protected :
            Cables():
                EventTemplate<Cable>()
            {
                
            }
        public :
            static CablesRef create(){
                return CablesRef( new Cables() );
            }
            virtual ~Cables(){
                
            }
            void draw() {
                for (auto const& [key, cable] : cables){
                    cable->draw();
                }
            };
            void addCable(PlugRef input, PlugRef output){
                CableRef cable = Cable::create(input, output);
                cables[output->getName(true)+">>>"+input->getName(true)] = cable;
                eventTrigger({"add", cable});
            }
        };//class Cables
        typedef std::shared_ptr<class Cables> CablesRef;
    }//namespace UI {
}//namespace ORAGE {
#endif /* Cables_ui_h */
