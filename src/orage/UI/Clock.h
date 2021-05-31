#ifndef UI_Clock_h
#define UI_Clock_h

#include "IView.h"
#include "Slider.h"

class UIClock : public IView {
    typedef std::shared_ptr<class UIClock> UIClockRef;
    UIClock(string name, ci::vec2 origin, ci::vec2 size);
    public :
        static UIClockRef create(string name, ci::vec2 origin, ci::vec2 size){
            return UIClockRef( new UIClock(name, origin, size) );
        }
        virtual void update() override;
        virtual void draw() override;
        virtual ~UIClock();
};

//////////////////////////////////

using namespace std;
using namespace ci;
using namespace ci::gl;

typedef shared_ptr<class UIClock> UIClockRef;

UIClock::UIClock(string name, vec2 origin, vec2 size) :
    IView(name, origin, size)
{
    ViewRef btn = addSubView("bang", View::create(name, {10, 10}, {10, 10}));
    ViewRef slider = addSubView("bpm", UISlider::create(name, {10, 30}, {100, 15}));
    btn->bgColor = Theme::bgActiveColor;
    View::bgColor = Theme::bgDisactiveColor;
    
    on("enter", [&](BaseEvent event) -> void{
        bgColor = Theme::bgActiveColor;
    });
    on("leave", [&](BaseEvent event) -> void{
        bgColor = Theme::bgDisactiveColor;
    });
}


UIClock::~UIClock(){
}

void UIClock::update(){
   IView::update();
}

void UIClock::draw(){
    View::draw();
}

#endif /* UI_Clock_h */
