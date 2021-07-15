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
        
        template<Layout T = Layout::Normal>
        class Number : public IView {
            typedef shared_ptr<class Number<T>> NumberRef;
            typedef COMMON::MouseEvent<View> MouseEvt;
            Surface  valueSurface;
            
            Number(string name, string type="Number") :
                IView(name, type)
            {
                setSize(Theme<T>::NumberSize);
                setBgColor(Theme<T>::bgActiveColor);
                
                ORAGE::UI::ViewRef iWrapper = wrapperBuilder("inputs");
                iWrapper->setSize(Theme<T>::PlugWrapperSize(getSize()));
                iWrapper->setPos(Theme<T>::PlugWrapperPos(getSize(), PlugType::Input));
                iWrapper->addView(PlugInput::create("input"));
                
                ORAGE::UI::ViewRef oWrapper = wrapperBuilder("outputs");
                oWrapper->setSize(Theme<T>::PlugWrapperSize(getSize()));
                oWrapper->setPos(Theme<T>::PlugWrapperPos(getSize(), PlugType::Output));
                oWrapper->addView(PlugOutput::create("output"));
                
                addEventListener("enter", boost::bind(&Number::onEnter, this, _1));
                addEventListener("leave", boost::bind(&Number::onLeave, this, _1));
            }
            
            UI::ViewRef wrapperBuilder(string name){
                switch(T){
                    case Layout::Normal : return addView(Inline::create(name));
                    case Layout::Lite :  return  addView(Incolumn::create(name));
                }
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
                simple.setFont( Theme<T>::getFont("bold") );
                simple.setColor( Color::white() );
                simple.addLine( value );
                valueSurface = simple.render( true, View::PREMULT ) ;
            }
        };//class Number
    }//namespace UI {
}//namespace ORAGE {
#endif /* UINumber_h */
