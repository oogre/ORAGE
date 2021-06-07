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

struct CustomEvent{
    string name;
    ci::app::MouseEvent mouseEvent;
    IViewRef target;
    std::time_t timeStamp;
    ivec2 oldMousePos;
    CustomEvent(string name, ci::app::MouseEvent mouseEvent, IViewRef target, ivec2 oldMousePos) :
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
        typedef boost::signals2::signal<bool(CustomEvent)> CustomEventSignal;
        typedef std::map<string, CustomEventSignal> MapType;
        MapType sigMap;
    
        bool wasisInside = false;
        bool isDown = false;
        bool isDraging = false;
        vec2 oldMousePos;
        std::time_t tAtDown;
        std::time_t tAtUp;
    
        static ci::app::MouseEvent lastMouseEvent;
        static bool initialized;
    
        CustomEventSignal * getSignal(string type);
        void eventTrigger(string type, ci::app::MouseEvent mouseEvent, ivec2 oldMousePos);
        bool onEnter(CustomEvent event);
        bool onLeave(CustomEvent event);
        bool onDrag(CustomEvent event);
    protected :
        IView(string name, ci::vec2 origin, ci::vec2 size);
    public :
        static IViewRef create(string name, ci::vec2 origin, ci::vec2 size){
            return IViewRef( new IView(name, origin, size) );
        }
        
        virtual void update() override;
        virtual ~IView();
    
        void addEventListener(const string type, const CustomEventSignal::slot_type slot);
        template<typename Callable>
        void removeEventListener(const string type, Callable slot);
    
    
};

//////////////////////////////////////////////

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace boost::signals2;

typedef signal<bool(CustomEvent)> CustomEventSignal;

MouseEvent IView::lastMouseEvent;
bool IView::initialized = false;

IView::IView(string name, ci::vec2 origin, ci::vec2 size) :
    View(name, origin, size)
{
    sigMap.insert(MapType::value_type({"over", CustomEventSignal()}));
    sigMap.insert(MapType::value_type({"enter", CustomEventSignal()}));
    sigMap.insert(MapType::value_type({"leave", CustomEventSignal()}));
    sigMap.insert(MapType::value_type({"wheel", CustomEventSignal()}));
    sigMap.insert(MapType::value_type({"up", CustomEventSignal()}));
    sigMap.insert(MapType::value_type({"down", CustomEventSignal()}));
    sigMap.insert(MapType::value_type({"click", CustomEventSignal()}));
    sigMap.insert(MapType::value_type({"doubleClick", CustomEventSignal()}));
    sigMap.insert(MapType::value_type({"longClick", CustomEventSignal()}));
    sigMap.insert(MapType::value_type({"dragStart", CustomEventSignal()}));
    sigMap.insert(MapType::value_type({"drag", CustomEventSignal()}));
    sigMap.insert(MapType::value_type({"dragEnd", CustomEventSignal()}));
    
    getWindow()->getSignalMouseMove().connect(100-getDepth(), [&]( MouseEvent event ){
        ivec2 mousePos = event.getPos();
        if(isInside(mousePos)){
            if(!wasisInside){
                eventTrigger("enter", event, oldMousePos);
            }
            eventTrigger("over", event, oldMousePos);
            
            wasisInside = true;
        }else if(wasisInside){
            eventTrigger("leave", event, oldMousePos);
            wasisInside = false;
        }
        oldMousePos = mousePos;
    });
    
    getWindow()->getSignalMouseWheel().connect(100-getDepth(), [&]( MouseEvent event ){
        ivec2 mousePos = event.getPos();
        if(isInside(mousePos)){
            eventTrigger("wheel", event, oldMousePos);
        }
        oldMousePos = mousePos;
    });
    
    getWindow()->getSignalMouseDrag().connect(100-getDepth(), [&]( MouseEvent event ){
        ivec2 mousePos = event.getPos();
        if(isDown){
            eventTrigger("drag", event, oldMousePos);
        }
        oldMousePos = mousePos;
    });
    
    getWindow()->getSignalMouseDown().connect(100-getDepth(), [&]( MouseEvent event ){
        ivec2 mousePos = event.getPos();
        if(isInside(mousePos)){
            eventTrigger("down", event, oldMousePos);
            tAtDown = std::time(0);
            isDown = true;
        }
        oldMousePos = mousePos;
    });
    
    getWindow()->getSignalMouseUp().connect(100-getDepth(), [&]( MouseEvent event ){
        vec2 mousePos = event.getPos();
        if(isInside(mousePos)){
            eventTrigger("up", event, oldMousePos);
            tAtUp = std::time(0);
            if(tAtUp - tAtDown < 1){
                eventTrigger("click", event, oldMousePos);
            }else{
                eventTrigger("longClick", event, oldMousePos);
            }
        }
        else if(isDown){
            eventTrigger("leave", event, oldMousePos);
            wasisInside = false;
        }
        isDown = false;
        oldMousePos = mousePos;
    });
    
    addEventListener("enter", boost::bind(&IView::onEnter, this, _1));
    addEventListener("leave", boost::bind(&IView::onLeave, this, _1));
    addEventListener("drag", boost::bind(&IView::onDrag, this, _1));
    
}

IView::~IView(){
    removeEventListener("enter", boost::bind(&IView::onEnter, this, _1));
    removeEventListener("leave", boost::bind(&IView::onLeave, this, _1));
}

bool IView::onEnter(CustomEvent event){
    setBgColor(Theme::bgActiveColor);
    cout<<event.target->getName(true)<<" : "<<event.name << " @ " << event.timeStamp <<endl;
    return true;
}

bool IView::onLeave(CustomEvent event){
    setBgColor(Theme::bgDisactiveColor);
    cout<<event.target->getName(true)<<" : "<<event.name<< " @ " << event.timeStamp <<endl;
    return true;
}

bool IView::onDrag(CustomEvent event){
    cout<<event.target->getName(true)<<" : "<<event.name<< " @ " << event.timeStamp <<endl;
    return true;
}

void IView::addEventListener(const string type, const CustomEventSignal::slot_type slot){
    getSignal(type)->connect(100-getDepth(), slot);
}

template<typename Callable>
void IView::removeEventListener(const string type, Callable slot){
    getSignal(type)->disconnect(slot);
}

CustomEventSignal * IView::getSignal(string type){
    MapType::iterator signal = sigMap.find(type);
    if(signal == sigMap.end())throw invalid_argument( "getSignal needs type to be recognized" );
    return &(signal->second);
}

void IView::eventTrigger(string type, MouseEvent mouseEvent, ivec2 oldMousePos){
    CustomEventSignal * signal = getSignal(type);
    CustomEvent event = {
        type,
        mouseEvent,
        static_pointer_cast<IView>(shared_from_this()),
        oldMousePos
    };
    ((*signal)(event));
}

void IView::update(){
    View::update();
}


#endif /* IView_h */
