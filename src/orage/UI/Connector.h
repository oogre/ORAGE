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
    Connector(ci::vec2 origin, ci::vec2 size, std::shared_ptr<class Parameter<T>> parameter);
    bool onEnter(IViewEvent event);
    bool onLeave(IViewEvent event);
    std::shared_ptr<class Parameter<T>> parameter;
public :
    static ConnectorRef create(ci::vec2 origin, ci::vec2 size, std::shared_ptr<class Parameter<T>> parameter){
        return ConnectorRef( new Connector(origin, size, parameter) );
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
Connector<T>::Connector(vec2 origin, vec2 size, shared_ptr<class Parameter<T>> parameter) :
IView(origin, size)
{
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
    translate( bounds.getUpperLeft() );
    
    color( ci::ColorA(1, 1, 1, open ? 0.7 : 0.3) );
    drawSolidRect({0, 0, bounds.getWidth(), bounds.getHeight()});
    popModelView();
}

#endif /* Connector_h */
