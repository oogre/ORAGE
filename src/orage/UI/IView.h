//
//  IView.h
//  Orage
//
//  Created by Vincent Evrard on 27/05/21.
//

#ifndef IView_h
#define IView_h

#include "View.h"

namespace ORAGE {
    namespace UI {
        using namespace std;
        using namespace ci;
        using namespace ci::app;
        
        class IView : public View {
            typedef shared_ptr<class IView> IViewRef;
            typedef COMMON::MouseEvent<View> MouseEvt;
            typedef COMMON::EventTemplate<View, MouseEvt> MouseEvtHandler;
            
            
            bool wasisInside = false;
            bool isDown = false;
            bool isDraging = false;
            vec2 oldMousePos;
            time_t tAtDown;
            time_t tAtUp;
            vector<ci::signals::Connection> connections;
            ColorA strokeColor = Theme::strokeDisactiveColor;
            
            void onMouseMove(ci::app::MouseEvent mouseEvent){
                ivec2 mousePos = mouseEvent.getPos();
                if(isInside(mousePos)){
                    if(!wasisInside){
                        eventTrigger("enter", mouseEvent, oldMousePos);
                    }
                    eventTrigger("over", mouseEvent, oldMousePos);
                    
                    wasisInside = true;
                }else if(wasisInside){
                    eventTrigger("leave", mouseEvent, oldMousePos);
                    wasisInside = false;
                }
                oldMousePos = mousePos;
            }//void onMouseMove
            
            void onMouseWheel(ci::app::MouseEvent mouseEvent){
                ivec2 mousePos = mouseEvent.getPos();
                if(isInside(mousePos)){
                    eventTrigger("wheel", mouseEvent, oldMousePos);
                }
                oldMousePos = mousePos;
            }//void onMouseWheel
            
            void onMouseDrag(ci::app::MouseEvent mouseEvent){
                ivec2 mousePos = mouseEvent.getPos();
                if(isDown){
                    if(!isDraging){
                        eventTrigger("dragStart", mouseEvent, oldMousePos);
                        isDraging = true;
                    }
                    eventTrigger("drag", mouseEvent, oldMousePos);
                }
                oldMousePos = mousePos;
            }//void onMouseDrag
            
            void onMouseDown(ci::app::MouseEvent mouseEvent){
                ivec2 mousePos = mouseEvent.getPos();
                if(isInside(mousePos)){
                    eventTrigger("down", mouseEvent, oldMousePos);
                    tAtDown = std::time(0);
                    isDown = true;
                }
                oldMousePos = mousePos;
            }//void onMouseDown
            
            void onMouseUp(ci::app::MouseEvent mouseEvent){
                vec2 mousePos = mouseEvent.getPos();
                
                
                if(isInside(mousePos)){
                    eventTrigger("up", mouseEvent, oldMousePos);
                    tAtUp = std::time(0);
                    if(tAtUp - tAtDown < 1){
                        eventTrigger("click", mouseEvent, oldMousePos);
                    }else{
                        eventTrigger("longClick", mouseEvent, oldMousePos);
                    }
                }
                else if(isDown){
                    eventTrigger("up", mouseEvent, oldMousePos);
                    eventTrigger("leave", mouseEvent, oldMousePos);
                    wasisInside = false;
                }
                if(isDraging){
                    eventTrigger("dragEnd", mouseEvent, oldMousePos);
                    isDraging = false;
                }
                isDown = false;
                oldMousePos = mousePos;
            }//void onMouseUp
        protected :
            IView(string name, string type = "IView"):
                View(name, type)
            {
                    connections.push_back(getWindow()->getSignalMouseMove().connect(100-getDepth(), boost::bind(&IView::onMouseMove, this, _1)));
                    connections.push_back(getWindow()->getSignalMouseWheel().connect(100-getDepth(), boost::bind(&IView::onMouseWheel, this, _1)));
                    connections.push_back(getWindow()->getSignalMouseDrag().connect(100-getDepth(), boost::bind(&IView::onMouseDrag, this, _1)));
                    connections.push_back(getWindow()->getSignalMouseDown().connect(100-getDepth(), boost::bind(&IView::onMouseDown, this, _1)));
                    connections.push_back(getWindow()->getSignalMouseUp().connect(100-getDepth(), boost::bind(&IView::onMouseUp, this, _1)));
                    addEventListener("enter", [&](MouseEvt event) -> bool {
                        strokeColor = Theme::strokeActiveColor;
                        return true;
                    });
                    addEventListener("leave", [&](MouseEvt event) -> bool {
                        strokeColor = Theme::strokeDisactiveColor;
                        return true;
                    });
            }
        public :
            static IViewRef create(string name){
                return IViewRef( new IView(name) );
            }
            virtual ~IView(){
                for(auto connection = connections.begin() ; connection != connections.end() ; ){
                    connection->disconnect();
                    connections.erase(connections.begin());
                }
            }
            void eventTrigger(string type, ci::app::MouseEvent mouseEvent, ivec2 oldMousePos){
                MouseEvtHandler::eventTrigger({
                    type,
                    mouseEvent,
                    as<View>(),
                    oldMousePos
                });
            }
            virtual void draw() override {
                View::draw();
                color( strokeColor );
                drawStrokedRect({0, 0, bounds.getWidth()+1, bounds.getHeight()+1});
            }
        };//class IView
        typedef shared_ptr<class IView> IViewRef;
    }//namespace UI {
}//namespace ORAGE {
#endif /* IView_h */
