#ifndef UI_Slider_h
#define UI_Slider_h

#include "IView.h"

// We'll create a new Cinder Application by deriving from the App class.
class UISlider : public IView  {
    typedef std::shared_ptr<class UISlider> UISliderRef;
    UISlider(ci::vec2 origin, ci::vec2 size);
    ViewRef cursor;
    ViewRef frame;
    public :
        static UISliderRef create(ci::vec2 origin, ci::vec2 size){
            return UISliderRef( new UISlider(origin, size) );
        }
        virtual void update() override;
        virtual void draw() override;
        virtual ~UISlider();
        void setCursor(float v);
};

//////////////////////////////////

using namespace std;
using namespace ci;
using namespace ci::gl;

typedef std::shared_ptr<class UISlider> UISliderRef;

UISlider::UISlider(vec2 origin, vec2 size) :
IView(origin, size)
{
    frame = addView("frame", View::create(origin+vec2(1, 1), size-vec2(2, 2)));
    frame->bgColor = Theme::bgDisactiveColor;
    cursor = frame->addSubView("cursor", View::create({1, 1}, {size.y-4,size.y-4}));
    on("enter", [&](BaseEvent event) -> void{
        bgColor = Theme::bgDisactiveColor;
        frame->bgColor = Theme::bgActiveColor;
        cursor->bgColor = Theme::bgDisactiveColor;
    });
    on("leave", [&](BaseEvent event) -> void{
        bgColor = Theme::bgActiveColor;
        frame->bgColor = Theme::bgDisactiveColor;
        cursor->bgColor = Theme::bgActiveColor;
    });
}

void UISlider::setCursor(float v){
    cursor->setPos({lerp(1.f, frame->getSize().x - cursor->getSize().x - 1.f, v) , 1});
}

UISlider::~UISlider(){
}

void UISlider::update(){
    IView::update();
}

void UISlider::draw(){
    View::draw();
}

#endif /* UI_Slider_h */
