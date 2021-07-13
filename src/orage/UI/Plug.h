//
//  Connector.h
//  Orage
//
//  Created by Vincent Evrard on 10/06/21.
//

#ifndef Plug_h
#define Plug_h

#include "IView.h"

namespace ORAGE {
    namespace UI {
        using namespace std;
        
        class Plug : public IView  {
            typedef shared_ptr<class Plug> PlugRef;
            typedef COMMON::MouseEvent<View> MouseEvt;
            enum TYPE { INPUT, OUTPUT };
            TYPE type;
            Plug(string name) :
                IView(name)
            {
                setSize(Theme::plugSize);
            }
        public :
            static PlugRef create(string name){
                return PlugRef( new Plug(name) );
            }
            virtual ~Plug(){
                
            }
        };//class Plug
        typedef std::shared_ptr<class Plug> PlugRef;
    }//namespace UI {
}//namespace ORAGE {

//
//template<typename T>
//void Connector<T>::draw(){
//    if(isInside() && _size != SIZE_OPEN){
//        setBgColor(Theme::ConnectorActiveColor);
//        _size.x = lerp(_size.x, SIZE_OPEN.x, SIZE_SPEED);
//        _size.y = lerp(_size.y, SIZE_OPEN.y, SIZE_SPEED);
//        setSize(_size);
//    }else if(!isInside() && _size != SIZE_CLOSE){
//        setBgColor(Theme::ConnectorDisactiveColor);
//        _size.x = lerp(_size.x, SIZE_CLOSE.x, SIZE_SPEED);
//        _size.y = lerp(_size.y, SIZE_CLOSE.y, SIZE_SPEED);
//        setSize(_size);
//    }
//
//    IView::draw();
//}

#endif /* Connector_h */
