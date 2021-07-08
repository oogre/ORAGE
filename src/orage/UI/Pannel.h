//
//  Pannel.h
//  Orage
//
//  Created by Vincent Evrard on 26/05/21.
//

#ifndef Pannel_h
#define Pannel_h

#include "IView.h"

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
            
            Pannel(string name, string type = "Pannel") :
                IView(name, type)
            {
                addView(IView::create("handle"))->addEventListener("drag", boost::bind(&Pannel::onDrag, this, _1));
                
                addEventListener("enter", boost::bind(&Pannel::onEnter, this, _1));
                addEventListener("leave", boost::bind(&Pannel::onLeave, this, _1));
                TextLayout simple;
                simple.setFont( Theme::getFont("bold") );
                simple.setColor( Color::white() );
                simple.addLine( name );
                titleTex = gl::Texture2d::create( simple.render( true, PREMULT ) );
                
                setBgColor(Theme::bgDisactiveColor);
            }
            bool onDrag(mouseEvt event){
                ivec2 dist = event.mouseEvent.getPos() - event.oldMousePos;
                move(dist);
                return true;
            }
            bool onEnter(mouseEvt event){
                forEach([&](ORAGE::COMMON::NodeRef node) -> void{
                    node->as<View>()->open = true;
                });
                return true;
            }
            bool onLeave(mouseEvt event){
                forEach([&](ORAGE::COMMON::NodeRef node) -> void{
                    node->as<View>()->open = false;
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
                color( ci::ColorA(1, 1, 1, 0.2) );
                drawSolidRect({0, 0, bounds.getWidth(), bounds.getHeight()});
                //    forEach([&](NodeRef node) -> void{
                //        pushModelView();
                //        node->as<View>()->draw();
                //        popModelView();
                //    });
                //View::draw();
                color( Color::white() );
                gl::draw( titleTex, vec2( 0, 0 ) );
                popModelView();
            };
        };//class Pannel
        typedef shared_ptr<class Pannel> PannelRef;
    }//namespace UI {
}//namespace ORAGE {
#endif /* Pannel_h */
