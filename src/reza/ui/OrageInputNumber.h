//
//  OrageInputNumber.h
//  ORAGE
//
//  Created by Vincent Evrard on 11/12/21.
//

#ifndef OrageInputNumber_h
#define OrageInputNumber_h
#include "UI.h"

namespace reza {
    namespace ui {
        using namespace ci;
        using namespace ci::app;
        using namespace std;
        
        class OrageInputNumber : public TextInput {
            typedef std::shared_ptr<OrageInputNumber> OrageInputNumberRef;
            vec2 oldDragPos;
            bool _isDragging = false;
            std::function<void( std::string )> mCallbackIncrement = []( std::string){};
            std::function<void( std::string )> mCallbackDecrement = []( std::string){};
        public :
            static OrageInputNumberRef create( std::string name, const TextInput::Format &format = TextInput::Format())
            {
                return OrageInputNumberRef( new OrageInputNumber( name, format ) );
            }
            
            OrageInputNumber( std::string name, const TextInput::Format &format = TextInput::Format()) :
                TextInput(name, format)
            {
           
            }
            
            virtual ~OrageInputNumber(){
            }
            virtual void setIncCallback( const std::function<void( std::string )> &callback ){
                mCallbackIncrement =callback;
            };
            virtual void setDecCallback( const std::function<void( std::string )> &callback ){
                mCallbackDecrement = callback;
            };
            virtual void keyDown( ci::app::KeyEvent &event ) override {
                TextInput::keyDown(event);
                if(! mClicked )return;
                switch( event.getCode() ) {
                    case KeyEvent::KEY_UP:
                        mCallbackIncrement(getValue());
                        break;
                    case KeyEvent::KEY_DOWN:
                        mCallbackDecrement(getValue());
                        break;
                }
            }
            virtual void mouseDrag( ci::app::MouseEvent &event ) override {
                if(_isDragging){
                    vec2 dragPos = event.getPos();
                    vec2 dist = dragPos - oldDragPos;
                    if(dist.y > 0){
                        mCallbackDecrement(getValue());
                    }
                    if(dist.y < 0){
                        mCallbackIncrement(getValue());
                    }
                    oldDragPos = dragPos;
                }
            }
            virtual void mouseDown( ci::app::MouseEvent &event ) override {
                TextInput::mouseDown(event);
                if(! mClicked )return;
                _isDragging = true;
                oldDragPos = event.getPos();
            }
            
            virtual void mouseUp( ci::app::MouseEvent &event ) override {
                TextInput::mouseDown(event);
                _isDragging = false;
            }
        };//OrageInputNumber
        typedef std::shared_ptr<OrageInputNumber> OrageInputNumberRef;
    }// ui
}// reza

#endif /* OrageInputNumber_h */
