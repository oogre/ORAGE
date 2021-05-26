#ifndef UI_Clock_h
#define UI_Clock_h

#include "View.h"

class UIClock : public View {
    typedef std::shared_ptr<class UIClock> UIClockRef;
    UIClock(ci::vec2 origin, ci::vec2 size);
    public :
    static UIClockRef create(ci::vec2 origin, ci::vec2 size){
        return UIClockRef( new UIClock(origin, size) );
    }
    virtual void update() override;
    virtual void draw() override;
    virtual ~UIClock() override;
};

//////////////////////////////////

using namespace std;
using namespace ci;
using namespace ci::gl;

typedef shared_ptr<class UIClock> UIClockRef;

UIClock::UIClock(vec2 origin, vec2 size) :
    View(origin, size)
{
    ViewRef btn = addSubView("bang", View::create({10, 10}, {10, 10}));
    btn->bgColor = Theme::bgActiveColor;
    View::bgColor = Theme::bgDisactiveColor;
}


UIClock::~UIClock(){
}

void UIClock::update(){
    View::update();
}

void UIClock::draw(){
    View::draw();
}

#endif /* UI_Clock_h */