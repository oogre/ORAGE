#ifndef UI_Slider_h
#define UI_Slider_h

#include "IView.h"
#include "../core/Parameter.h"

// We'll create a new Cinder Application by deriving from the App class.
template<typename T>
class UISlider : public IView  {
    typedef std::shared_ptr<class UISlider> UISliderRef;
    UISlider(string name, ci::vec2 origin, ci::vec2 size);
    ViewRef cursor;
    ViewRef frame;
    bool onEnter(CustomEvent event);
    bool onLeave(CustomEvent event);
    bool onWheel(CustomEvent event);
    bool onDown(CustomEvent event);
    shared_ptr<class Parameter<T>> paramter;
public :
    static UISliderRef create(string name, ci::vec2 origin, ci::vec2 size){
        return UISliderRef( new UISlider(name, origin, size) );
    }
    virtual void update() override;
    virtual void draw() override;
    virtual ~UISlider();
    void setCursor(float v);
    
    std::shared_ptr<class UISlider<T>> setParameter(std::shared_ptr<class Parameter<T>> paramter);
};

//////////////////////////////////

using namespace std;
using namespace ci;
using namespace ci::gl;

typedef std::shared_ptr<class UISlider<int>> UISliderRefI;
typedef std::shared_ptr<class UISlider<float>> UISliderRefF;

template<typename T>
bool UISlider<T>::onEnter(CustomEvent event){
    setBgColor(Theme::bgDisactiveColor);
    frame->setBgColor(Theme::bgActiveColor);
    cursor->setBgColor(Theme::bgDisactiveColor);
    return true;
}

template<typename T>
bool UISlider<T>::onLeave(CustomEvent event){
    setBgColor(Theme::bgActiveColor);
    frame->setBgColor(Theme::bgDisactiveColor);
    cursor->setBgColor(Theme::bgActiveColor);
    return true;
}

template<typename T>
bool UISlider<T>::onWheel(CustomEvent event){
    paramter->setValue(paramter->getValue() - event.mouseEvent.getWheelIncrement());
    return true;
}

template<typename T>
bool UISlider<T>::onDown(CustomEvent event){
    ivec2 dist = event.mouseEvent.getPos() - ivec2(getPos(true));
    float n_x_dist = dist.x/getSize().x;
    paramter->setNormalizedValue(n_x_dist);
    return true;
}

template<typename T>
UISlider<T>::UISlider(string name, vec2 origin, vec2 size) :
    IView(name, origin, size)
{
    frame = addView("frame", View::create(name, origin+vec2(1, 1), size-vec2(2, 2)));
    frame->setBgColor(Theme::bgDisactiveColor);
    cursor = frame->addSubView<View>("cursor", View::create(name, {1, 1}, {size.y-4,size.y-4}));
    
    addEventListener("enter", boost::bind(&UISlider<T>::onEnter, this, _1));
    addEventListener("leave", boost::bind(&UISlider<T>::onLeave, this, _1));
}

template<typename T>
void UISlider<T>::setCursor(float v){
    cursor->setPos({lerp(1.f, frame->getSize().x - cursor->getSize().x - 1.f, v) , 1});
}

template<typename T>
UISlider<T>::~UISlider(){
    removeEventListener("enter", boost::bind(&UISlider<T>::onEnter, this, _1));
    removeEventListener("leave", boost::bind(&UISlider<T>::onLeave, this, _1));
}

template<typename T>
void UISlider<T>::update(){
    IView::update();
}

template<typename T>
void UISlider<T>::draw(){
    View::draw();
}

template<typename T>
shared_ptr<class UISlider<T>> UISlider<T>::setParameter(shared_ptr<class Parameter<T>> p){
    removeEventListener("wheel", boost::bind(&UISlider::onWheel, this, _1));
    paramter = p;
    setCursor(paramter->getValue(true));
    addEventListener("wheel", boost::bind(&UISlider::onWheel, this, _1));
    addEventListener("down", boost::bind(&UISlider::onDown, this, _1));
    addEventListener("drag", boost::bind(&UISlider::onDown, this, _1));
    return static_pointer_cast<UISlider>(shared_from_this());
}


#endif /* UI_Slider_h */
