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

// We'll create a new Cinder Application by deriving from the App class.
template<typename T>
class Connector : public IView  {
    typedef std::shared_ptr<class Connector> ConnectorRef;
    static vec2 SIZE_OPEN;
    static vec2 SIZE_CLOSE;
    static float SIZE_SPEED;
    vec2 _size;
    Connector(ci::vec2 origin, std::shared_ptr<class Parameter<T>> parameter);
    bool onEnter(IViewEvent event);
    bool onLeave(IViewEvent event);
    std::shared_ptr<class Parameter<T>> parameter;
public :
    static ConnectorRef create(ci::vec2 origin, std::shared_ptr<class Parameter<T>> parameter){
        return ConnectorRef( new Connector(origin, parameter) );
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
Connector<T>::Connector(vec2 origin, shared_ptr<class Parameter<T>> parameter) :
IView(origin, SIZE_OPEN)
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
bool Connector<T>::onEnter(IViewEvent event){
    cout<<parameter->getName()<< " enter" << endl;
}

template<typename T>
bool Connector<T>::onLeave(IViewEvent event){
    cout<<parameter->getName()<< " leave" << endl;
}

template<typename T>
void Connector<T>::draw(){
    pushModelView();
    translate( bounds.getCenter() );
    
    color( ci::ColorA(1, 1, 1, open ? 0.7 : 0.3) );
    drawSolidRect({-1 * (bounds.getWidth()*0.5f), -1 * (bounds.getHeight()*0.5f), bounds.getWidth()*0.5f, bounds.getHeight()*0.5f});
    popModelView();
    
    
    //size.x = lerp(size.x, open ? SIZE_OPEN.x : SIZE_CLOSE.x, SIZE_SPEED);
    //size.y = lerp(size.y, open ? SIZE_OPEN.y : SIZE_CLOSE.y, SIZE_SPEED);
    //setSize(size);
    
    
    
}

#endif /* Connector_h */
