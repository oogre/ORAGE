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
    UISlider(ci::vec2 origin, ci::vec2 size, std::shared_ptr<class Parameter<T>> parameter);
    ViewRef cursor;
    ViewRef slider;
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
    shared_ptr<class Parameter<T>> paramter;
    void onParamChange(ParameterEvent<T> event);
public :
    static UISliderRef create(ci::vec2 origin, ci::vec2 size, std::shared_ptr<class Parameter<T>> parameter){
        return UISliderRef( new UISlider(origin, size, parameter) );
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
float UISlider<T>::OFFSET_SPEED = 0.25;

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
UISlider<T>::UISlider(vec2 origin, vec2 size, shared_ptr<class Parameter<T>> parameter) :
    IView(origin, size)
{
    slider = addSubView<View>("slider", View::create(vec2(1, 1), size-vec2(2, 2)));
    slider->setBgColor(Theme::bgDisactiveColor);
    cursor = slider->addSubView<View>("cursor", View::create({1, 1}, vec2(size.y, size.y)-vec2(4, 4)));
    
    connectorIn = addSubView<View>("connectorIn", Connector<T>::create(vec2( -7, size.y/2 - 2), vec2(6, 6), parameter));
    connectorOut = addSubView<View>("connectorOut", Connector<T>::create(vec2(size.x + 1, size.y/2 - 2), vec2(6, 6), parameter));
    
    this->paramter = parameter;
    render(true);
    
    TextLayout simple;
    simple.setFont( getFont("bold") );
    simple.setColor( Color::white() );
    simple.addLine(paramter->getName());
    nameTex = gl::Texture2d::create( simple.render( true, View::PREMULT ) ) ;
    
    addEventListener("enter", boost::bind(&UISlider<T>::onEnter, this, _1));
    addEventListener("leave", boost::bind(&UISlider<T>::onLeave, this, _1));
    addEventListener("wheel", boost::bind(&UISlider::onWheel, this, _1));
    addEventListener("down", boost::bind(&UISlider::onDown, this, _1));
    addEventListener("drag", boost::bind(&UISlider::onDown, this, _1));
    paramter->addEventListener(boost::bind(&UISlider<T>::onParamChange, this, _1));
}

template<typename T>
void UISlider<T>::render(bool force){
    float nValue = (paramter->getValue() - paramter->getMin()) * paramter->getRatio();
    
    cursor->setPos({lerp(1.f, slider->getSize().x - cursor->getSize().x - 1.f, nValue) , 1});
    if(open || force){
        TextLayout simple;
        simple.setFont( getFont("bold") );
        simple.setColor( Color::white() );
        stringstream stream;
        stream << std::fixed << std::setprecision(1) << paramter->getValue();
        simple.addLine( stream.str() );
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
    IView::draw();
    if(open){
        color( ColorA(1, 1, 1, offset/OFFSET_OPEN ) );
        gl::draw( gl::Texture2d::create( valueTex ), vec2( - nameTex->getWidth() - offset , 0 ) );
        gl::draw(  nameTex, vec2( getSize().x + offset, 0 ) );
    }
    offset = lerp(offset, open ? OFFSET_OPEN : OFFSET_CLOSE, OFFSET_SPEED);
    popModelView();
}


#endif /* UI_Slider_h */
