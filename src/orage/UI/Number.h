//
//  Number.h
//  Orage
//
//  Created by Vincent Evrard on 2/07/21.
//

#ifndef UINumber_h
#define UINumber_h

#include "Plug.h"
#include "Inline.h"
#include "IView.h"

namespace ORAGE {
    namespace UI {
        using namespace std;
        using namespace ci;
        using namespace ci::gl;
        using json = nlohmann::json;
        
        class Number : public IView {
            typedef shared_ptr<class Number> NumberRef;
            typedef COMMON::MouseEvent<View> MouseEvt;
            Surface  valueSurface;
            
            Number(string name, string type="Number") :
                IView(name, type)
            {
                setSize(Theme::NumberSize);
                setBgColor(Theme::bgActiveColor);
                
                ORAGE::UI::ViewRef inputs = addView(Inline::create("inputs"));
                inputs->setSize({getSize().x, 5});
                inputs->setPos({0, -7});
                inputs->addView(PlugInput::create("input"));
                
                ORAGE::UI::ViewRef outputs = addView(Inline::create("outputs"));
                outputs->setSize({getSize().x, 5});
                outputs->setPos({0, getSize().y+2});
                outputs->anchor = TOP_LEFT;
                outputs->addView(PlugOutput::create("output"));
                
                addEventListener("enter", boost::bind(&Number::onEnter, this, _1));
                addEventListener("leave", boost::bind(&Number::onLeave, this, _1));
            }
            bool onDragStart(MouseEvt event){
                addEventListener("drag", boost::bind(&Number::onDrag, this, _1));
                addEventListener("dragEnd", boost::bind(&Number::onDragEnd, this, _1));
                return true;
            }
            bool onDrag(MouseEvt event){
                ivec2 dist = event.mouseEvent.getPos() - event.oldMousePos;
                if(module->is<CORE::NumberI>()){
                    module->setValue(module->getValue() - dist.y);
                }else{
                    module->setValue(module->getValue() - dist.y * 0.01);
                }
                return true;
            }
            bool onDragEnd(MouseEvt event){
                removeEventListener("drag", boost::bind(&Number::onDrag, this, _1));
                removeEventListener("dragEnd", boost::bind(&Number::onDragEnd, this, _1));
                return true;
            }
            bool onEnter(MouseEvt event){
                addEventListener("dragStart", boost::bind(&Number::onDragStart, this, _1));
                return true;
            }
            bool onLeave(MouseEvt event){
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
                module->removeEventListener("change", boost::bind(&Number::onChange, this, _1));
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
            virtual void setModule(CORE::ModuleRef module) override {
                View::setModule(module);
                module->addEventListener("change", boost::bind(&Number::onChange, this, _1));
                render( module->getStringValue() );
            }
            void onChange(COMMON::Event<CORE::Module> event){
                render( event.target->getStringValue() );
            }
            void render(string value){
                TextLayout simple;
                simple.setFont( Theme::getFont("bold") );
                simple.setColor( Color::white() );
                simple.addLine( value );
                valueSurface = simple.render( true, View::PREMULT ) ;
            }
        };//class Number
        typedef shared_ptr<class Number> NumberRef;
    }//namespace UI {
}//namespace ORAGE {
#endif /* UINumber_h */
