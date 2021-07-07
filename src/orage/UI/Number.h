//
//  Number.h
//  Orage
//
//  Created by Vincent Evrard on 2/07/21.
//

#ifndef UINumber_h
#define UINumber_h


#include "IView.h"

namespace ORAGE {
    namespace UI {
        using namespace std;
        using namespace ci;
        using namespace ci::gl;
        using json = nlohmann::json;
        
        class Number : public IView {
            typedef shared_ptr<class Number> NumberRef;
            
            Surface  valueSurface;
            
            Number(string name, string type="Number") :
                IView(name, type)
            {
                setSize(Theme::NumberSize);
                addEventListener("enter", boost::bind(&Number::onEnter, this, _1));
                addEventListener("leave", boost::bind(&Number::onLeave, this, _1));
                setBgColor(Theme::bgActiveColor);
            }
            bool onDragStart(IViewEvent event){
                addEventListener("drag", boost::bind(&Number::onDrag, this, _1));
                addEventListener("dragEnd", boost::bind(&Number::onDragEnd, this, _1));
                return true;
            }
            bool onDrag(IViewEvent event){
                ivec2 dist = event.mouseEvent.getPos() - event.oldMousePos;
                if(parameter->is<CORE::NumberI>()){
                    parameter->setValue(parameter->getValue() - dist.y);
                }else{
                    parameter->setValue(parameter->getValue() - dist.y * 0.01);
                }
                return true;
            }
            bool onDragEnd(IViewEvent event){
                removeEventListener("drag", boost::bind(&Number::onDrag, this, _1));
                removeEventListener("dragEnd", boost::bind(&Number::onDragEnd, this, _1));
                return true;
            }
            bool onEnter(IViewEvent event){
                addEventListener("dragStart", boost::bind(&Number::onDragStart, this, _1));
                return true;
            }
            bool onLeave(IViewEvent event){
                removeEventListener("dragStart", boost::bind(&Number::onDragStart, this, _1));
                return true;
            }
        public :
            static NumberRef create(string name){
                return NumberRef( new Number(name) );
            }
            virtual ~Number() {
                removeEventListener("enter", boost::bind(&Number::onEnter, this, _1));
                removeEventListener("leave", boost::bind(&Number::onLeave, this, _1));
                removeEventListener("dragStart", boost::bind(&Number::onDragStart, this, _1));
                removeEventListener("drag", boost::bind(&Number::onDrag, this, _1));
                removeEventListener("dragEnd", boost::bind(&Number::onDragEnd, this, _1));
                parameter->removeEventListener("change", boost::bind(&Number::onChange, this, _1));
            }
            virtual void draw() override {
                pushModelView();
                IView::draw();
                color( Color::black() );
                Texture2dRef tex = Texture2d::create(valueSurface);
                vec2 pos = vec2( getSize().x-valueSurface.getWidth() - 5, int(getSize().y-valueSurface.getHeight()) >> 1 );
                gl::draw( tex, pos );
                popModelView();
            }
            virtual void setParameter(CORE::ParameterRef parameter) override {
                View::setParameter(parameter);
                parameter->addEventListener("change", boost::bind(&Number::onChange, this, _1));
                render( parameter->getStringValue() );
            }
            void onChange(CORE::ParameterEvent event){
                render( event.target->getStringValue() );
            }
            void render(string value){
                TextLayout simple;
                simple.setFont( Theme::getFont("bold") );
                simple.setColor( Color::white() );
                simple.addLine( value );
                valueSurface = simple.render( true, View::PREMULT ) ;
            }
        };//class Number : public IView {
        typedef shared_ptr<class Number> NumberRef;
    }//namespace UI {
}//namespace ORAGE {
#endif /* UINumber_h */
