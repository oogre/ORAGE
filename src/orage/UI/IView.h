//
//  IView.h
//  Orage
//
//  Created by Vincent Evrard on 27/05/21.
//

#ifndef IView_h
#define IView_h

#include "View.h"
#include <boost/signals2.hpp>
// https://scicomp.ethz.ch/public/manual/Boost/1.55.0/signals2.pdf

namespace ORAGE {
    namespace UI {
        using namespace std;
        using namespace ci;
        using namespace ci::app;
        using namespace boost::signals2;
        
        class IView;
        typedef std::shared_ptr<class IView> IViewRef;

        struct IViewEvent{
            string name;
            MouseEvent mouseEvent;
            IViewRef target;
            time_t timeStamp;
            ivec2 oldMousePos;
            IViewEvent(){}
            IViewEvent(string name, IViewRef target) : name(name), target(target){};
            IViewEvent(string name, MouseEvent mouseEvent, IViewRef target, ivec2 oldMousePos) :
                name(name),
                mouseEvent(mouseEvent),
                target(target),
                timeStamp(time(0)),
                oldMousePos(oldMousePos)
            {};
            string to_string();
        };//struct IViewEvent
    
        class IView : public View {
            typedef shared_ptr<class IView> IViewRef;
            typedef signal<bool(IViewEvent)> IViewEventSignal;
            typedef map<string, IViewEventSignal> MapType;
            MapType sigMap;
            bool wasisInside = false;
            bool isDown = false;
            bool isDraging = false;
            vec2 oldMousePos;
            time_t tAtDown;
            time_t tAtUp;
            vector<ci::signals::Connection> connections;
            ColorA strokeColor = Theme::strokeDisactiveColor;
            
            void onMouseMove(MouseEvent mouseEvent){
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
            
            void onMouseWheel(MouseEvent mouseEvent){
                ivec2 mousePos = mouseEvent.getPos();
                if(isInside(mousePos)){
                    eventTrigger("wheel", mouseEvent, oldMousePos);
                }
                oldMousePos = mousePos;
            }//void onMouseWheel
            
            void onMouseDrag(MouseEvent mouseEvent){
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
            
            void onMouseDown(MouseEvent mouseEvent){
                ivec2 mousePos = mouseEvent.getPos();
                if(isInside(mousePos)){
                    eventTrigger("down", mouseEvent, oldMousePos);
                    tAtDown = std::time(0);
                    isDown = true;
                }
                oldMousePos = mousePos;
            }//void onMouseDown
            
            void onMouseUp(MouseEvent mouseEvent){
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
                    sigMap.insert(MapType::value_type({"over", IViewEventSignal()}));
                    sigMap.insert(MapType::value_type({"enter", IViewEventSignal()}));
                    sigMap.insert(MapType::value_type({"leave", IViewEventSignal()}));
                    sigMap.insert(MapType::value_type({"wheel", IViewEventSignal()}));
                    sigMap.insert(MapType::value_type({"up", IViewEventSignal()}));
                    sigMap.insert(MapType::value_type({"down", IViewEventSignal()}));
                    sigMap.insert(MapType::value_type({"click", IViewEventSignal()}));
                    sigMap.insert(MapType::value_type({"doubleClick", IViewEventSignal()}));
                    sigMap.insert(MapType::value_type({"longClick", IViewEventSignal()}));
                    sigMap.insert(MapType::value_type({"dragStart", IViewEventSignal()}));
                    sigMap.insert(MapType::value_type({"drag", IViewEventSignal()}));
                    sigMap.insert(MapType::value_type({"dragEnd", IViewEventSignal()}));
                    connections.push_back(getWindow()->getSignalMouseMove().connect(100-getDepth(), boost::bind(&IView::onMouseMove, this, _1)));
                    connections.push_back(getWindow()->getSignalMouseWheel().connect(100-getDepth(), boost::bind(&IView::onMouseWheel, this, _1)));
                    connections.push_back(getWindow()->getSignalMouseDrag().connect(100-getDepth(), boost::bind(&IView::onMouseDrag, this, _1)));
                    connections.push_back(getWindow()->getSignalMouseDown().connect(100-getDepth(), boost::bind(&IView::onMouseDown, this, _1)));
                    connections.push_back(getWindow()->getSignalMouseUp().connect(100-getDepth(), boost::bind(&IView::onMouseUp, this, _1)));
                    addEventListener("enter", [&](IViewEvent event) -> bool {
                        strokeColor = Theme::strokeActiveColor;
                        return true;
                    });
                    addEventListener("leave", [&](IViewEvent event) -> bool {
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
            void eventTrigger(string type, MouseEvent mouseEvent, ivec2 oldMousePos){
                IViewEvent event{
                    type,
                    mouseEvent,
                    as<IView>(),
                    oldMousePos
                };
                MapType::iterator signal = sigMap.find(type);
                if(signal == sigMap.end())return ;
                (sigMap.at(type))(event);
            }
            void eventTrigger(string type, IViewRef target = nullptr){
                IViewEvent event;
                if(target == nullptr){
                    event = IViewEvent(type, as<IView>());
                }else{
                    event = IViewEvent(type, target);
                }
                MapType::iterator signal = sigMap.find(type);
                if(signal == sigMap.end())return ;
                (sigMap.at(type))(event);
            }
            void addEventListener(const string type, const IViewEventSignal::slot_type slot){
                if(sigMap.find(type) == sigMap.end()){
                    sigMap.insert(MapType::value_type({type, IViewEventSignal()}));
                }
                sigMap.at(type).connect(slot);
            }
            template<typename Callable>
            void removeEventListener(const string type, Callable slot){
                if(sigMap.find(type) != sigMap.end()){
                    sigMap.at(type).disconnect(slot);
                }
            }
            virtual ORAGE::UI::ViewRef addView(ORAGE::UI::ViewRef view) override {
                auto t = View::addView(view);
                eventTrigger("add", view->as<IView>());
                return t;
            }
            virtual void draw() override {
                View::draw();
                color( strokeColor );
                drawStrokedRect({0, 0, bounds.getWidth()+1, bounds.getHeight()+1});
            }
        };//class IView
        typedef signal<bool(IViewEvent)> IViewEventSignal;
        typedef shared_ptr<class IView> IViewRef;
        
        string IViewEvent::to_string() {
            return name + " : " + target->getName(true);
        }
    }//namespace UI {
}//namespace ORAGE {
#endif /* IView_h */
