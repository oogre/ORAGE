//
//  IView.h
//  Orage
//
//  Created by Vincent Evrard on 27/05/21.
//

#ifndef IView_h
#define IView_h

#include "View.h"

class IView : public View {
    protected :
        struct BaseEvent{
            string name;
            ci::app::MouseEvent mouseEvent;
            BaseEvent(string name, ci::app::MouseEvent mouseEvent) :
            name(name),
            mouseEvent(mouseEvent)
            {
            };
        };
        struct OverEvent : public BaseEvent{
            OverEvent(ci::app::MouseEvent mouseEvent) :
                BaseEvent("over", mouseEvent)
            {
            };
        };
        struct EnterEvent : public BaseEvent{
            EnterEvent(ci::app::MouseEvent mouseEvent) :
            BaseEvent("enter", mouseEvent)
            {
            };
        };
        struct LeaveEvent : public BaseEvent{
            LeaveEvent(ci::app::MouseEvent mouseEvent) :
            BaseEvent("leave", mouseEvent)
            {
            };
        };
    private :
        typedef std::function<void(BaseEvent)> BaseFnc;
        std::map<string, std::vector<BaseFnc>> baseFncs;
        bool wasisInside = false;
        static ci::app::MouseEvent lastMouseEvent;
    protected :
        IView(string name, ci::vec2 origin, ci::vec2 size);
    public :
        IView* on(string type, BaseFnc fnc);
        virtual void update() override;
};

//////////////////////////////////////////////

using namespace std;
using namespace ci;
using namespace ci::app;

typedef shared_ptr<class IView> IViewRef;

MouseEvent IView::lastMouseEvent;

IView::IView(string name, ci::vec2 origin, ci::vec2 size) :
    View(name, origin, size)
{
    getWindow()->getSignalMouseMove().connect( [&]( MouseEvent event ){
        IView::lastMouseEvent = event;
    });
    getWindow()->getSignalMouseDrag().connect( [&]( MouseEvent event ){
        IView::lastMouseEvent = event;
    });
}

IView* IView::on(string type, BaseFnc fnc){
    baseFncs[type].push_back(fnc);
    return this;
}

void IView::update(){
    vec2 p = getPos(true);
    Rectf b = {p, bounds.getSize() + p};
    if(b.contains(IView::lastMouseEvent.getPos())){
        for(auto fnc : baseFncs["over"]){
            fnc(OverEvent(IView::lastMouseEvent));
        }
        if(!wasisInside){
            for(auto fnc : baseFncs["enter"]){
                fnc(EnterEvent(IView::lastMouseEvent));
            }
        }
        wasisInside = true;
    }else{
        if(wasisInside){
            for(auto fnc : baseFncs["leave"]){
                fnc(LeaveEvent(IView::lastMouseEvent));
            }
        }
        wasisInside = false;
    }
    View::update();
}


#endif /* IView_h */
