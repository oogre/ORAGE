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
class IView;
typedef std::shared_ptr<class IView> IViewRef;

struct CustomEvent{
    string name;
    ci::app::MouseEvent mouseEvent;
    IViewRef target;
    CustomEvent(string name, ci::app::MouseEvent mouseEvent, IViewRef target) :
    name(name),
    mouseEvent(mouseEvent),
    target(target)
    {
    };
};

class IView : public View, public std::enable_shared_from_this<IView> {
        typedef std::shared_ptr<class IView> IViewRef;
        typedef boost::signals2::signal<void(CustomEvent)> CustomEventSignal;
        typedef std::map<string, CustomEventSignal> MapType;
        MapType sigMap;
    
        bool wasisInside = false;
    
        static ci::app::MouseEvent lastMouseEvent;
        static IViewRef lastTarget;
        static bool initialized;
    
    protected :
        IView(string name, ci::vec2 origin, ci::vec2 size);
    public :
        static IViewRef create(string name, ci::vec2 origin, ci::vec2 size){
            return IViewRef( new IView(name, origin, size) );
        }
        
        virtual void update() override;
        virtual ~IView();
    
        CustomEventSignal * getSignal(string type);
};

//////////////////////////////////////////////

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace boost::signals2;

typedef signal<void(CustomEvent)> CustomEventSignal;

MouseEvent IView::lastMouseEvent;
IViewRef IView::lastTarget;
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
    sigMap.insert(MapType::value_type({"doubleClick", CustomEventSignal()}));
    sigMap.insert(MapType::value_type({"dragEnd", CustomEventSignal()}));
    
    if(!IView::initialized){
        IView::initialized = true;
        getWindow()->getSignalMouseMove().connect( [&]( MouseEvent event ){
            IView::lastMouseEvent = event;
        });
        getWindow()->getSignalMouseWheel().connect( [&]( MouseEvent event ){
            IView::lastMouseEvent = event;
        });
        getWindow()->getSignalMouseDown().connect( [&]( MouseEvent event ){
            IView::lastMouseEvent = event;
        });
        getWindow()->getSignalMouseUp().connect( [&]( MouseEvent event ){
            IView::lastMouseEvent = event;
        });
    }
//    overSignal.connect([&](OverEvent event) -> void{
//        cout<<event.target->getName(true)<<" : over"<<endl;
//    });
    
    
    getSignal("enter")->connect([&](CustomEvent event) -> void{
        //if(IView::lastTarget == event.target){
            cout<<event.target->getName(true)<<" : "<<event.name<<endl;
        //}
    });
}

IView::~IView(){
    
}

CustomEventSignal * IView::getSignal(string type){
    return  &(sigMap[type]);
}

void IView::update(){
    vec2 mousePos = IView::lastMouseEvent.getPos();
//    if(isInside(mousePos)){
//        foreach<bool>([&](ViewRef sView, int key, int name) {
//            return sView->isInside(mousePos);
//        });
//
//
//        //for(auto [name, sView] : subViews){
//            //if(sView->isInside(mousePos))break;
//        //    cout<<typeid(sView).name()<<end;
//        //}
//        sigMap["over"]({
//            "over",
//            IView::lastMouseEvent,
//            shared_from_this()
//        });
//    }

    if(isInside(mousePos)){
        sigMap["over"]({
            "over",
            IView::lastMouseEvent,
            shared_from_this()
        });
        if(IView::lastMouseEvent.isLeft()){
            sigMap["down"]({
                "down",
                IView::lastMouseEvent,
                shared_from_this()
            });
        }
        if(IView::lastMouseEvent.getWheelIncrement() != 0){
            sigMap["wheel"]({
                "wheel",
                IView::lastMouseEvent,
                shared_from_this()
            });
        }
        if(!wasisInside){
            IView::lastTarget = shared_from_this();
            sigMap["enter"]({
                "enter",
                IView::lastMouseEvent,
                shared_from_this()
            });
        }
        wasisInside = true;
        
    }else{
        if(wasisInside){
            sigMap["leave"]({
                "leave",
                IView::lastMouseEvent,
                shared_from_this()
            });
        }
        wasisInside = false;
    }
    
    View::update();
}


#endif /* IView_h */
