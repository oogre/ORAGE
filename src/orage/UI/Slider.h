#ifndef UI_Slider_h
#define UI_Slider_h

#include "IView.h"
#include "Connector.h"
#include "../core/Parameter.h"
#include "Pannel.h"
#include <cmath>

// We'll create a new Cinder Application by deriving from the App class.
template<typename T>
class UISlider : public IView  {
    typedef std::shared_ptr<class UISlider> UISliderRef;
    UISlider(ci::vec2 origin, ci::vec2 size, std::shared_ptr<class Parameter<T>> parameter, ANCHOR anchor = TOP_LEFT);
    ViewRef cursor;
    IViewRef slider;
    IViewRef limitterSlider;
    ViewRef limitterCursor;
    ViewRef connectorIn;
    ViewRef connectorOut;
    Surface  valueTex;
    gl::Texture2dRef  nameTex;
    static float OFFSET_OPEN;
    static float OFFSET_CLOSE;
    static float OFFSET_SPEED;
    float offset = OFFSET_CLOSE;
    
    bool onEnter(IViewEvent event);
    bool onLeave(IViewEvent event);
    bool onWheel(IViewEvent event);
    bool onDown(IViewEvent event);

    bool onEnterLimitter(IViewEvent event);
    bool onLeaveLimitter(IViewEvent event);
    bool onWheelLimitter(IViewEvent event);
    bool onDownLimitter(IViewEvent event);
    
    shared_ptr<class Parameter<T>> paramter;
    void onParamChange(ParameterEvent<T> event);
public :
    static UISliderRef create(ci::vec2 origin, ci::vec2 size, std::shared_ptr<class Parameter<T>> parameter, ANCHOR anchor = TOP_LEFT){
        return UISliderRef( new UISlider(origin, size, parameter, anchor) );
    }
    virtual ~UISlider();
    void render(bool force = false);
    virtual void draw() override;
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
float UISlider<T>::OFFSET_OPEN = 11;
template<typename T>
float UISlider<T>::OFFSET_CLOSE = 0;
template<typename T>
float UISlider<T>::OFFSET_SPEED = 0.1;

template<typename T>
void UISlider<T>::onParamChange(ParameterEvent<T> event){
    render();
}

template<typename T>
bool UISlider<T>::onEnter(IViewEvent event){
    setBgColor(Theme::bgDisactiveColor);
    slider->setBgColor(Theme::bgActiveColor);
    cursor->setBgColor(Theme::bgDisactiveColor);
    connectorIn->open = true;
    connectorOut->open = true;
    slider->addEventListener("wheel", boost::bind(&UISlider::onWheel, this, _1));
    slider->addEventListener("down", boost::bind(&UISlider::onDown, this, _1));
    slider->addEventListener("drag", boost::bind(&UISlider::onDown, this, _1));
    return true;
}

template<typename T>
bool UISlider<T>::onLeave(IViewEvent event){
    setBgColor(Theme::bgActiveColor);
    slider->setBgColor(Theme::bgDisactiveColor);
    cursor->setBgColor(Theme::bgActiveColor);
    connectorIn->open = false;
    connectorOut->open = false;
    slider->removeEventListener("wheel", boost::bind(&UISlider::onWheel, this, _1));
    slider->removeEventListener("down", boost::bind(&UISlider::onDown, this, _1));
    slider->removeEventListener("drag", boost::bind(&UISlider::onDown, this, _1));
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
//render();
//}

template<typename T>
bool UISlider<T>::onEnterLimitter(IViewEvent event){
//    setBgColor(Theme::bgDisactiveColor);
//    slider->setBgColor(Theme::bgActiveColor);
//    cursor->setBgColor(Theme::bgDisactiveColor);
//    connectorIn->open = true;
//    connectorOut->open = true;
    limitterSlider->addEventListener("wheel", boost::bind(&UISlider::onWheelLimitter, this, _1));
    limitterSlider->addEventListener("down", boost::bind(&UISlider::onDownLimitter, this, _1));
    limitterSlider->addEventListener("drag", boost::bind(&UISlider::onDownLimitter, this, _1));
    return true;
}

template<typename T>
bool UISlider<T>::onLeaveLimitter(IViewEvent event){
//    setBgColor(Theme::bgActiveColor);
//    slider->setBgColor(Theme::bgDisactiveColor);
//    cursor->setBgColor(Theme::bgActiveColor);
//    connectorIn->open = false;
//    connectorOut->open = false;
    limitterSlider->removeEventListener("wheel", boost::bind(&UISlider::onWheelLimitter, this, _1));
    limitterSlider->removeEventListener("down", boost::bind(&UISlider::onDownLimitter, this, _1));
    limitterSlider->removeEventListener("drag", boost::bind(&UISlider::onDownLimitter, this, _1));
    return true;
}

template<typename T>
bool UISlider<T>::onWheelLimitter(IViewEvent event){
//    paramter->setValue(paramter->getValue() - event.mouseEvent.getWheelIncrement());
    return true;
}

template<typename T>
bool UISlider<T>::onDownLimitter(IViewEvent event){
    ivec2 dist = event.mouseEvent.getPos() - ivec2(getPos(true));
    float n_x_dist = dist.x/getSize().x;
    
    float dm = abs(paramter->getMin(true) - n_x_dist);
    float dM = abs(paramter->getMax(true) - n_x_dist);
    if(dm < dM){
      paramter->setNormalizedMin(n_x_dist);
    }else{
      paramter->setNormalizedMax(n_x_dist);
    }
//
    return true;
}

template<typename T>
UISlider<T>::UISlider(vec2 origin, vec2 size, shared_ptr<class Parameter<T>> parameter, View::ANCHOR anchor) :
    IView(origin, size, anchor)
{
    slider = addSubView<IView>("slider", IView::create(vec2(1, 1), size-vec2(2, 2)));
    slider->setBgColor(Theme::bgDisactiveColor);
    cursor = addSubView<View>("cursor", View::create(vec2(2, 2), vec2(size.y, size.y)-vec2(4, 4)));
    limitterSlider = addSubView<IView>("limitterSlider", IView::create(vec2(0, size.y+1), size-vec2(0, size.y - 6)));
    limitterSlider->setBgColor(Theme::bgDisactiveColor);
    limitterCursor = addSubView<View>("limitterCursor", View::create(vec2(0, size.y+1), size-vec2(0, size.y - 6)));
    
    
    connectorIn =  addSubView<View>("connectorIn",  Connector<T>::create( vec2(0, 0.5) * size, parameter, Connector<T>::INPUT, Connector<T>::CENTER_RIGHT));
    connectorOut = addSubView<View>("connectorOut", Connector<T>::create( vec2(1, 0.5) * size, parameter, Connector<T>::OUTPUT, Connector<T>::CENTER_LEFT));
    
    this->paramter = parameter;
    render(true);
     
    TextLayout simple;
    simple.setFont( getFont("bold") );
    simple.setColor( Color::white() );
    simple.addLine(paramter->getName());
    nameTex = gl::Texture2d::create( simple.render( true, View::PREMULT ) ) ;
    
    slider->addEventListener("enter", boost::bind(&UISlider<T>::onEnter, this, _1));
    slider->addEventListener("leave", boost::bind(&UISlider<T>::onLeave, this, _1));
    
    
    limitterSlider->addEventListener("enter", boost::bind(&UISlider<T>::onEnterLimitter, this, _1));
    limitterSlider->addEventListener("leave", boost::bind(&UISlider<T>::onLeaveLimitter, this, _1));
    
    paramter->addEventListener(boost::bind(&UISlider<T>::onParamChange, this, _1));
}

template<typename T>
void UISlider<T>::render(bool force){
    float t = paramter->getValue(true);
    cursor->setPos({lerp(2.f, slider->getSize().x - cursor->getSize().x , t) , cursor->getPos().y});
    
    limitterCursor->setPos({limitterSlider->getSize().x * (paramter->getMin(true)) , limitterCursor->getPos().y});
    limitterCursor->setSize({limitterSlider->getSize().x * (paramter->getMax(true) - paramter->getMin(true)) , limitterCursor->getSize().y});
    
    if(open || force){
        TextLayout simple;
        simple.setFont( getFont("bold") );
        simple.setColor( Color::white() );
        simple.addLine( paramter->getStringValue() );
        valueTex = simple.render( true, View::PREMULT ) ;
    }
}

template<typename T>
UISlider<T>::~UISlider(){
    removeEventListener("enter", boost::bind(&UISlider<T>::onEnter, this, _1));
    removeEventListener("leave", boost::bind(&UISlider<T>::onLeave, this, _1));
}

template<typename T>
void UISlider<T>::draw(){
    pushModelView();
    translate( getPos() );
    if(open){
        pushModelView();
        translate({offset, 0 });
        color(ColorA::white());
        gl::draw(nameTex, vec2( getSize().x , 0 ) );
        popModelView();
        
        pushModelView();
        translate({-offset, 0 });
        color(ColorA::white());
        gl::draw(gl::Texture2d::create( valueTex ), vec2( - nameTex->getWidth()  , 0 ) );
        popModelView();
    }
    offset = lerp(offset, open ? OFFSET_OPEN : OFFSET_CLOSE, OFFSET_SPEED);
    
    color( getBgColor() );
    drawSolidRect({0, 0, getSize().x, getSize().y});
    slider->draw();
    limitterSlider->draw();
    cursor->draw();
    limitterCursor->draw();
    connectorIn->draw();
    connectorOut->draw();
    popModelView();
}


#endif /* UI_Slider_h */
