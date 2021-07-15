//
//  Cable.h
//  Orage
//
//  Created by Vincent Evrard on 8/07/21.
//

#ifndef Cable_UI_h
#define Cable_UI_h

#include "Plug.h"
#include "Pannel.h"
//https://libcinder.org/docs/guides/path2d/part2.html
namespace ORAGE {
    namespace UI {
        using namespace std;
        using namespace ci;
        using namespace ci::gl;
        class Cable {
            typedef shared_ptr<class Cable> CableRef;
            typedef COMMON::MouseEvent<View> MouseEvt;
            Path2d  path;
            
            bool  over = false;
            PannelRef iPannel;
            PannelRef oPannel;
            vec2* mousePos;
            
        public :
            PlugInputRef input;
            PlugOutputRef output;
            
        protected :
            Cable(PlugInputRef input, PlugOutputRef output, vec2* mousePos = nullptr) :
                input(input),
                output(output),
                mousePos(mousePos)
            {
                iPannel = getPannel(input);
                oPannel = getPannel(output);
                (iPannel!=nullptr) && iPannel->addEventListener("move", boost::bind(&Cable::renderWrapper, this, _1));
                (oPannel!=nullptr) && oPannel->addEventListener("move", boost::bind(&Cable::renderWrapper, this, _1));
                render();
            }
            void renderWrapper(MouseEvt event){
                render();
            }
        public :
            PannelRef getPannel(ViewRef view){
                if(view == nullptr)return nullptr;
                UI::ViewRef parent = view->getParent<UI::View>([&](UI::ViewRef view) -> bool{
                    return view->is<Pannel>();
                });
                if(parent == nullptr)return nullptr;
                return parent->as<Pannel>();
            }
            static CableRef create(PlugInputRef input, PlugOutputRef output){
                return CableRef( new Cable(input, output) );
            }
            static CableRef create(PlugInputRef input, vec2* mousePos){
                return CableRef( new Cable(input, nullptr, mousePos) );
            }
            static CableRef create(PlugOutputRef output, vec2* mousePos){
                return CableRef( new Cable(nullptr, output, mousePos) );
            }
            virtual ~Cable(){
                (iPannel!=nullptr) && iPannel->removeEventListener("move", boost::bind(&Cable::renderWrapper, this, _1));
                (oPannel!=nullptr) && oPannel->removeEventListener("move", boost::bind(&Cable::renderWrapper, this, _1));
            }
            bool isInput(){
                return input != nullptr;
            }
            bool isOutput(){
                return output != nullptr;
            }
            virtual void draw() {
                if(mousePos!=nullptr){
                    render();
                }
                if(over){
                    gl::color( Color( 1.0f, 0.5f, 0.25f ) );
                }else{
                    gl::color( Color::white() );
                }
                gl::draw( path );
            }
            void render(){
                vec2 pStart = (output != nullptr) ? output->getCenter(true) : vec2(mousePos->x, mousePos->y);
                vec2 pStop  = (input  != nullptr) ? input->getCenter(true)  : vec2(mousePos->x, mousePos->y);
                vec2 pCenter = (pStart + pStop)*0.5f;
                vec2 p1 = vec2(pStart.x, pCenter.y);
                vec2 p2 = vec2(pStop.x, pCenter.y);
                path.clear();
                path.moveTo( pStart );
                path.curveTo( p1, p2, pStop );
            }
        };//class Cable
        typedef shared_ptr<class Cable> CableRef;
    }//namespace UI {
}//namespace ORAGE {
#endif /* Cable_UI_h */
