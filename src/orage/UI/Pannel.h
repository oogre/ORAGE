//
//  Pannel.h
//  Orage
//
//  Created by Vincent Evrard on 26/05/21.
//

#ifndef Pannel_h
#define Pannel_h

#include "IView.h"
#include "Incolumn.h"

namespace ORAGE {
    namespace UI {
        using namespace std;
        using namespace ci;
        using namespace ci::gl;
        using json = nlohmann::json;
        
        
        class Pannel : public IView {
            typedef shared_ptr<class Pannel> PannelRef;
            typedef COMMON::MouseEvent<View> mouseEvt;

            Texture2dRef titleTex;
            ORAGE::UI::IViewRef handle;
            ORAGE::UI::ViewRef parameters;
            Pannel(string name, string type = "Pannel") :
                IView(name, type)
            {
                handle = IView::create("handle");
                handle->setSize({getSize().x, 15});
                handle->addEventListener("drag", boost::bind(&Pannel::onDrag, this, _1));
                
                parameters = Incolumn::create("parameters");
                parameters->setSize({getSize().x, getSize().y});
                parameters->setPos({0, 0});
                parameters->setBgColor(ColorA(1, 0, 0, 1));
                parameters->addView(handle);
                addView(parameters);
                
                addEventListener("enter", boost::bind(&Pannel::onEnter, this, _1));
                addEventListener("leave", boost::bind(&Pannel::onLeave, this, _1));
                
                TextLayout simple;
                simple.setFont( Theme<Layout::Normal>::getFont("bold") );
                simple.setColor( Color::white() );
                simple.addLine( name );
                titleTex = gl::Texture2d::create( simple.render( true, PREMULT ) );
                
                setBgColor(Theme<Layout::Normal>::bgDisactiveColor);
            }
            bool onDrag(mouseEvt event){
                ivec2 dist = event.mouseEvent.getPos() - event.oldMousePos;
                move(dist);
                return true;
            }
            bool onEnter(mouseEvt event){
                forEach<UI::View>([&](UI::ViewRef view) -> void{
                    view->open = true;
                });
                return true;
            }
            bool onLeave(mouseEvt event){
                forEach<UI::View>([&](UI::ViewRef view) -> void{
                    view->open = false;
                });
                return true;
            }
        public :
            static PannelRef create(string name){
                return PannelRef( new Pannel(name) );
            }
            virtual ~Pannel() {
            };
            virtual void draw() override {
                pushModelView();
                translate( bounds.getUpperLeft() );
                
                handle->draw();
                parameters->draw();
                
                color( Color::black() );
                gl::draw( titleTex, vec2( 0, 0 ) );
                popModelView();
            };
        };//class Pannel
        typedef shared_ptr<class Pannel> PannelRef;
    }//namespace UI {
}//namespace ORAGE {
#endif /* Pannel_h */
