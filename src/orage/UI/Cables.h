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
            ci::signals::Connection mouseMoveHandler;
            vec2 mousePos;
            CablesWrapper cables;
            CableRef tempCable;
        protected :
            Cables():
                EventTemplate<Cable>()
            {
                mouseMoveHandler = getWindow()->getSignalMouseMove().connect(boost::bind(&Cables::onMouseMove, this, _1));
            }
        public :
            static CablesRef create(){
                return CablesRef( new Cables() );
            }
            virtual ~Cables(){
                mouseMoveHandler.disconnect();
            }
            void onMouseMove(ci::app::MouseEvent event){
                mousePos = event.getPos();
                //https://github.com/cinder/Cinder/blob/master/include/cinder/Path2d.h
                //over = path.calcDistance(event.getPos()) < 20;
            }
            void draw() {
                for (auto const& [key, cable] : cables){
                    cable->draw();
                }
                if(tempCable != nullptr){
                    tempCable->draw(mousePos);
                }
            };
            void addCable(PlugInputRef input, PlugOutputRef output, bool trigEvent = true){
                CableRef cable = Cable::create(input, output);
                cables[output->getName(true)+">>>"+input->getName(true)] = cable;
                if(trigEvent){
                    eventTrigger({"add", cable});
                }
            }
            void addCable(PlugInputRef input, bool trigEvent = true){
                if(tempCable && tempCable->isOutput()){
                    addCable(input, tempCable->output, trigEvent);
                    tempCable = nullptr;
                }else{
                    tempCable = Cable::create(input);
                }
            }
            void addCable(PlugOutputRef output, bool trigEvent = true){
                if(tempCable && tempCable->isInput()){
                    addCable(tempCable->input, output, trigEvent);
                    tempCable = nullptr;
                }else{
                    tempCable = Cable::create(output);
                }
            }
        };//class Cables
        typedef std::shared_ptr<class Cables> CablesRef;
    }//namespace UI {
}//namespace ORAGE {
#endif /* Cables_ui_h */
