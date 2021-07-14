//
//  Cable.h
//  Orage
//
//  Created by Vincent Evrard on 8/07/21.
//

#ifndef Cable_UI_h
#define Cable_UI_h

#include "Plug.h"
//https://libcinder.org/docs/guides/path2d/part2.html
namespace ORAGE {
    namespace UI {
        using namespace std;
        using namespace ci;
        using namespace ci::gl;
        class Cable {
            typedef shared_ptr<class Cable> CableRef;
            Path2d  path;
            
            bool  over = false;
            
        public :
            PlugInputRef input;
            PlugOutputRef output;
        protected :
            Cable(PlugInputRef input, PlugOutputRef output) :
                input(input),
                output(output)
            {
                render();
            }
            
        public :
            static CableRef create(PlugInputRef input, PlugOutputRef output){
                return CableRef( new Cable(input, output) );
            }
            static CableRef create(PlugInputRef input){
                return CableRef( new Cable(input, nullptr) );
            }
            static CableRef create(PlugOutputRef output){
                return CableRef( new Cable(nullptr, output) );
            }
            virtual ~Cable(){
            }
            bool isInput(){
                return input != nullptr;
            }
            bool isOutput(){
                return output != nullptr;
            }
            virtual void draw(vec2 mPos = vec2(0, 0)) {
                if(output == nullptr || input == nullptr){
                    render(mPos);
                }
                if(over){
                    gl::color( Color( 1.0f, 0.5f, 0.25f ) );
                }else{
                    gl::color( Color::white() );
                }
                gl::draw( path );
            }
            void render(vec2 mPos = vec2(0, 0)){
                vec2 pStart = (output != nullptr) ? output->getCenter(true) : mPos;
                vec2 pStop  = (input  != nullptr) ? input->getCenter(true)  : mPos;
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
