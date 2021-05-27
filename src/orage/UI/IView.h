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
    struct BaseEvent{
        BaseEvent(string name) :
        name(name)
        {
        };
        string name;
    };
    public :
        struct OverEvent : public BaseEvent{
            OverEvent(string name) :
                BaseEvent(name)
            {
            };
        };
    private :
        typedef std::function<void(OverEvent)> overFnc;
        std::vector<overFnc> overFncs;
        bool overActivate = false;
    protected :
        IView(ci::vec2 origin, ci::vec2 size) :
            View(origin, size)
        {
        };
    public :
        void onOver(overFnc fnc);
};

///////////////////////

using namespace std;
using namespace ci;
using namespace ci::app;

typedef shared_ptr<class IView> IViewRef;

void IView::onOver(overFnc fnc){
    if(!overActivate){
        overActivate = true;
        getWindow()->getSignalMouseMove().connect( [&]( MouseEvent event ){
            if(bounds.contains(event.getPos())){
               // fnc({"over"});
            }
        });
    }
}


#endif /* IView_h */
