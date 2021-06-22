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
class IView;
typedef std::shared_ptr<class IView> IViewRef;

struct IViewEvent{
    string name;
    ci::app::MouseEvent mouseEvent;
    IViewRef target;
    std::time_t timeStamp;
    ivec2 oldMousePos;
    IViewEvent(string name, ci::app::MouseEvent mouseEvent, IViewRef target, ivec2 oldMousePos) :
        name(name),
        mouseEvent(mouseEvent),
        target(target),
        timeStamp(std::time(0)),
        oldMousePos(oldMousePos)
    {
    };
};

class IView : public View {
        typedef std::shared_ptr<class IView> IViewRef;
        typedef boost::signals2::signal<bool(IViewEvent)> IViewEventSignal;
        typedef std::map<string, IViewEventSignal> MapType;
        MapType sigMap;
    
        bool wasisInside = false;
        bool isDown = false;
        bool isDraging = false;
        vec2 oldMousePos;
        std::time_t tAtDown;
        std::time_t tAtUp;
    
        static ci::app::MouseEvent lastMouseEvent;
        static bool initialized;
    
        IViewEventSignal * getSignal(string type);
        void eventTrigger(string type, ci::app::MouseEvent mouseEvent, ivec2 oldMousePos);
        bool onVerobse(IViewEvent event);
        void onMouseMove(ci::app::MouseEvent mouseEvent);
        void onMouseWheel(ci::app::MouseEvent mouseEvent);
        void onMouseDrag(ci::app::MouseEvent mouseEvent);
        void onMouseDown(ci::app::MouseEvent mouseEvent);
        void onMouseUp(ci::app::MouseEvent mouseEvent);
        std::vector<ci::signals::Connection> connections;
    protected :
        IView(ci::vec2 origin, ci::vec2 size, View::ANCHOR anchor = TOP_LEFT);
    public :
    static IViewRef create(ci::vec2 origin, ci::vec2 size, View::ANCHOR anchor = TOP_LEFT){
            return IViewRef( new IView(origin, size, anchor) );
        }
    
        virtual ~IView();
    
        void addEventListener(const string type, const IViewEventSignal::slot_type slot);
        template<typename Callable>
        void removeEventListener(const string type, Callable slot);
    
    
};

//////////////////////////////////////////////

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace boost::signals2;

typedef signal<bool(IViewEvent)> IViewEventSignal;

MouseEvent IView::lastMouseEvent;
bool IView::initialized = false;


void IView::onMouseMove(MouseEvent mouseEvent){
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
}
void IView::onMouseWheel(MouseEvent mouseEvent){
    ivec2 mousePos = mouseEvent.getPos();
    if(isInside(mousePos)){
        eventTrigger("wheel", mouseEvent, oldMousePos);
    }
    oldMousePos = mousePos;
}
void IView::onMouseDrag(MouseEvent mouseEvent){
    ivec2 mousePos = mouseEvent.getPos();
    if(isDown){
        eventTrigger("drag", mouseEvent, oldMousePos);
    }
    oldMousePos = mousePos;
}
void IView::onMouseDown(MouseEvent mouseEvent){
    ivec2 mousePos = mouseEvent.getPos();
    if(isInside(mousePos)){
        eventTrigger("down", mouseEvent, oldMousePos);
        tAtDown = std::time(0);
        isDown = true;
    }
    oldMousePos = mousePos;
}
void IView::onMouseUp(MouseEvent mouseEvent){
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
        eventTrigger("leave", mouseEvent, oldMousePos);
        wasisInside = false;
    }
    isDown = false;
    oldMousePos = mousePos;
}

IView::IView(ci::vec2 origin, ci::vec2 size, View::ANCHOR anchor) :
    View(origin, size, anchor)
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
    
//    addEventListener("over", boost::bind(&IView::onVerobse, this, _1));
//    addEventListener("enter", boost::bind(&IView::onVerobse, this, _1));
//    addEventListener("leave", boost::bind(&IView::onVerobse, this, _1));
//    addEventListener("wheel", boost::bind(&IView::onVerobse, this, _1));
//    addEventListener("up", boost::bind(&IView::onVerobse, this, _1));
//    addEventListener("down", boost::bind(&IView::onVerobse, this, _1));
//    addEventListener("click", boost::bind(&IView::onVerobse, this, _1));
//    addEventListener("doubleClick", boost::bind(&IView::onVerobse, this, _1));
//    addEventListener("longClick", boost::bind(&IView::onVerobse, this, _1));
//    addEventListener("dragStart", boost::bind(&IView::onVerobse, this, _1));
//    addEventListener("drag", boost::bind(&IView::onVerobse, this, _1));
//    addEventListener("dragEnd", boost::bind(&IView::onVerobse, this, _1));
}

IView::~IView(){
    for(auto connection = connections.begin() ; connection != connections.end() ; ){
        connection->disconnect();
        connections.erase(connections.begin());
    }
//    removeEventListener("over", boost::bind(&IView::onVerobse, this, _1));
//    removeEventListener("enter", boost::bind(&IView::onVerobse, this, _1));
//    removeEventListener("leave", boost::bind(&IView::onVerobse, this, _1));
//    removeEventListener("wheel", boost::bind(&IView::onVerobse, this, _1));
//    removeEventListener("up", boost::bind(&IView::onVerobse, this, _1));
//    removeEventListener("down", boost::bind(&IView::onVerobse, this, _1));
//    removeEventListener("click", boost::bind(&IView::onVerobse, this, _1));
//    removeEventListener("doubleClick", boost::bind(&IView::onVerobse, this, _1));
//    removeEventListener("longClick", boost::bind(&IView::onVerobse, this, _1));
//    removeEventListener("dragStart", boost::bind(&IView::onVerobse, this, _1));
//    removeEventListener("drag", boost::bind(&IView::onVerobse, this, _1));
//    removeEventListener("dragEnd", boost::bind(&IView::onVerobse, this, _1));
}

bool IView::onVerobse(IViewEvent event){
    cout<<event.target->getName(true)<<" : "<<event.name << " @ " << event.timeStamp <<endl;
    return true;
}

void IView::addEventListener(const string type, const IViewEventSignal::slot_type slot){
    getSignal(type)->connect(100-getDepth(), slot);
}

template<typename Callable>
void IView::removeEventListener(const string type, Callable slot){
    getSignal(type)->disconnect(slot);
}

IViewEventSignal * IView::getSignal(string type){
    MapType::iterator signal = sigMap.find(type);
    if(signal == sigMap.end())throw invalid_argument( "getSignal needs type to be recognized" );
    return &(signal->second);
}

void IView::eventTrigger(string type, MouseEvent mouseEvent, ivec2 oldMousePos){
    IViewEventSignal * signal = getSignal(type);
    IViewEvent event = {
        type,
        mouseEvent,
        static_pointer_cast<IView>(shared_from_this()),
        oldMousePos
    };
    ((*signal)(event));
}


#endif /* IView_h */
