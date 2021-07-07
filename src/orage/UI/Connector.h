//
//  Connector.h
//  Orage
//
//  Created by Vincent Evrard on 10/06/21.
//

#ifndef Connector_h
#define Connector_h

#include "IView.h"
#include "../core/Parameter.h"
//#include "../core/Cable.h"

// We'll create a new Cinder Application by deriving from the App class.
template<typename T>
class Connector : public IView  {
    typedef std::shared_ptr<class Connector> ConnectorRef;
public :
    static vec2 SIZE_CLOSE;
    enum TYPE { INPUT, OUTPUT };
private :
    TYPE type;
    static vec2 SIZE_OPEN;
    static float SIZE_SPEED;
    bool inside = false;
    bool isInside();
    vec2 _size;
    Connector(ci::vec2 origin, std::shared_ptr<class Parameter<T>> parameter, TYPE type, View::ANCHOR anchor = TOP_LEFT);
    bool onEnter(IViewEvent event);
    bool onLeave(IViewEvent event);
    bool onClick(IViewEvent event);
    std::shared_ptr<class Parameter<T>> parameter;
public :
    static ConnectorRef create(ci::vec2 origin, std::shared_ptr<class Parameter<T>> parameter, TYPE type, View::ANCHOR anchor = TOP_LEFT){
        return ConnectorRef( new Connector(origin, parameter, type, anchor) );
    }
    virtual ~Connector();
    virtual void draw() override;
};


//////////////////////////////////

using namespace std;
using namespace ci;
using namespace ci::gl;

typedef Connector<int> ConnectorI;
typedef Connector<float> ConnectorF;
typedef std::shared_ptr<ConnectorI> ConnectorRefI;
typedef std::shared_ptr<ConnectorF> ConnectorRefF;

template<typename T>
vec2 Connector<T>::SIZE_OPEN = vec2(10, 10);
template<typename T>
vec2 Connector<T>::SIZE_CLOSE = vec2(3, 6);
template<typename T>
float Connector<T>::SIZE_SPEED = 0.1f;


template<typename T>
Connector<T>::Connector(vec2 origin, shared_ptr<class Parameter<T>> parameter, TYPE type, View::ANCHOR anchor) :
IView(origin, SIZE_CLOSE, anchor)
{
    _size = getSize();
    this->parameter = parameter;
    addEventListener("enter", boost::bind(&Connector<T>::onEnter, this, _1));
    addEventListener("leave", boost::bind(&Connector<T>::onLeave, this, _1));
}

template<typename T>
Connector<T>::~Connector(){
    
}

template<typename T>
bool Connector<T>::onClick(IViewEvent event){
    
    if(type == INPUT){
        
    }else if(type == OUTPUT){
        
    }
    
//    CableRef cable = Cable::create(parameter);
//    if(cable->isComplete()){
//        cable->getInput()->getParent(true)->addCable(cable);
//    }
    
    return true;
}

template<typename T>
bool Connector<T>::onEnter(IViewEvent event){
    addEventListener("down", boost::bind(&Connector<T>::onClick, this, _1));
    addEventListener("drag", boost::bind(&Connector<T>::onClick, this, _1));
    inside = true;
    return true;
}

template<typename T>
bool Connector<T>::onLeave(IViewEvent event){
    removeEventListener("down", boost::bind(&Connector<T>::onClick, this, _1));
    removeEventListener("drag", boost::bind(&Connector<T>::onClick, this, _1));
    inside = false;
    return true;
}

template<typename T>
bool Connector<T>::isInside(){
    return inside || open;
}

template<typename T>
void Connector<T>::draw(){
    if(isInside() && _size != SIZE_OPEN){
        setBgColor(Theme::ConnectorActiveColor);
        _size.x = lerp(_size.x, SIZE_OPEN.x, SIZE_SPEED);
        _size.y = lerp(_size.y, SIZE_OPEN.y, SIZE_SPEED);
        setSize(_size);
    }else if(!isInside() && _size != SIZE_CLOSE){
        setBgColor(Theme::ConnectorDisactiveColor);
        _size.x = lerp(_size.x, SIZE_CLOSE.x, SIZE_SPEED);
        _size.y = lerp(_size.y, SIZE_CLOSE.y, SIZE_SPEED);
        setSize(_size);
    }

    IView::draw();
}

#endif /* Connector_h */
