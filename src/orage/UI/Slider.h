#ifndef UI_Slider_h
#define UI_Slider_h

#include "IView.h"
#include "../core/Parameter.h"
#include <cmath>

// We'll create a new Cinder Application by deriving from the App class.
template<typename T>
class UISlider : public IView  {
    typedef std::shared_ptr<class UISlider> UISliderRef;
    UISlider(ci::vec2 origin, ci::vec2 size);
    ViewRef cursor;
    ViewRef slider;
    Surface  valueTex;
    
    bool onEnter(IViewEvent event);
    bool onLeave(IViewEvent event);
    bool onWheel(IViewEvent event);
    bool onDown(IViewEvent event);
    shared_ptr<class Parameter<T>> paramter;
    void onParamChange(ParameterEvent<T> event);
public :
    static UISliderRef create(ci::vec2 origin, ci::vec2 size){
        return UISliderRef( new UISlider(origin, size) );
    }
    virtual ~UISlider();
    void render();
    virtual void draw() override;
    std::shared_ptr<class UISlider<T>> setParameter(std::shared_ptr<class Parameter<T>> paramter);
};

//////////////////////////////////

using namespace std;
using namespace ci;
using namespace ci::gl;

typedef UISlider<int> UISliderI;
typedef UISlider<float> UISliderF;
typedef std::shared_ptr<UISliderI> UISliderRefI;
typedef std::shared_ptr<UISliderF> UISliderRefF;


template<typename T>
void UISlider<T>::onParamChange(ParameterEvent<T> event){
    render();
}

template<typename T>
bool UISlider<T>::onEnter(IViewEvent event){
    setBgColor(Theme::bgDisactiveColor);
    slider->setBgColor(Theme::bgActiveColor);
    cursor->setBgColor(Theme::bgDisactiveColor);
    return true;
}

template<typename T>
bool UISlider<T>::onLeave(IViewEvent event){
    setBgColor(Theme::bgActiveColor);
    slider->setBgColor(Theme::bgDisactiveColor);
    cursor->setBgColor(Theme::bgActiveColor);
    return true;
}

template<typename T>
bool UISlider<T>::onWheel(IViewEvent event){
    paramter->setValue(paramter->getValue() - event.mouseEvent.getWheelIncrement());
    return true;
}

template<typename T>
bool UISlider<T>::onDown(IViewEvent event){
    ivec2 dist = event.mouseEvent.getPos() - ivec2(getPos(true));
    float n_x_dist = dist.x/getSize().x;
    paramter->setNormalizedValue(n_x_dist);
    return true;
}

template<typename T>
UISlider<T>::UISlider(vec2 origin, vec2 size) :
    IView(origin, size)
{
    slider = addSubView<View>("slider", View::create(vec2(1, 1), size-vec2(2, 2)));
    slider->setBgColor(Theme::bgDisactiveColor);
    cursor = slider->addSubView<View>("cursor", View::create({1, 1}, vec2(size.y, size.y)-vec2(4, 4)));
    
    addEventListener("enter", boost::bind(&UISlider<T>::onEnter, this, _1));
    addEventListener("leave", boost::bind(&UISlider<T>::onLeave, this, _1));
    
}

template<typename T>
void UISlider<T>::render(){
    float nValue = (paramter->getValue() - paramter->getMin()) * paramter->getRatio();
    
    cursor->setPos({lerp(1.f, slider->getSize().x - cursor->getSize().x - 1.f, nValue) , 1});
    
    TextLayout simple;
    simple.setFont( getFont("bold") );
    simple.setColor( Color::black() );
    stringstream stream;
    stream << std::fixed << std::setprecision(1) << paramter->getValue();
    simple.addLine( stream.str() );
    valueTex = simple.render( true, View::PREMULT ) ;
}

template<typename T>
UISlider<T>::~UISlider(){
    removeEventListener("enter", boost::bind(&UISlider<T>::onEnter, this, _1));
    removeEventListener("leave", boost::bind(&UISlider<T>::onLeave, this, _1));
}

template<typename T>
shared_ptr<class UISlider<T>> UISlider<T>::setParameter(shared_ptr<class Parameter<T>> p){
    if(paramter!= nullptr){
        paramter->removeEventListener(boost::bind(&UISlider<T>::onParamChange, this, _1));
    }
    removeEventListener("wheel", boost::bind(&UISlider::onWheel, this, _1));
    paramter = p;
    render();
    addEventListener("wheel", boost::bind(&UISlider::onWheel, this, _1));
    addEventListener("down", boost::bind(&UISlider::onDown, this, _1));
    addEventListener("drag", boost::bind(&UISlider::onDown, this, _1));
    paramter->addEventListener(boost::bind(&UISlider<T>::onParamChange, this, _1));
    
    
    
    return static_pointer_cast<UISlider>(shared_from_this());
}

template<typename T>
void UISlider<T>::draw(){
    pushModelView();
    IView::draw();
    color( Color::white() );
    gl::draw( gl::Texture2d::create( valueTex ), vec2( 0, 0 ) );
    popModelView();
}


#endif /* UI_Slider_h */
