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
    
}

template<typename T>
Connector<T>::~Connector(){
    
}

template<typename T>
void Connector<T>::draw(){
    IView::draw();
    /*
    pushModelView();
    IView::draw();
    if(open){
        color( ColorA(1, 1, 1, offset/OFFSET_OPEN ) );
        gl::draw( gl::Texture2d::create( valueTex ), vec2( - nameTex->getWidth() - offset , 0 ) );
        gl::draw(  nameTex, vec2( getSize().x + offset, 0 ) );
    }
    offset = lerp(offset, open ? OFFSET_OPEN : OFFSET_CLOSE, OFFSET_SPEED);
    popModelView();
     */
}

#endif /* Connector_h */
