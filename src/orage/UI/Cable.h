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
        class Cable  {
            typedef shared_ptr<class Cable> CableRef;
            Path2d  path;
            bool  over = false;
            ci::signals::Connection mouseMoveHandler;
        public :
            PlugRef input;
            PlugRef output;
        protected :
            Cable(PlugRef input, PlugRef output) :
                input(input),
                output(output)
            {
                render();
                //mouseMoveHandler = getWindow()->getSignalMouseMove().connect(boost::bind(&Cable::onMouseMove, this, _1));
            }
        public :
            static CableRef create(PlugRef input, PlugRef output){
                return CableRef( new Cable(input, output) );
            }
            virtual ~Cable(){
            }
            void onMouseMove(ci::app::MouseEvent event){
                //https://github.com/cinder/Cinder/blob/master/include/cinder/Path2d.h
                //over = path.calcDistance(event.getPos()) < 20;
            }
            virtual void draw() {
                if(over){
                gl::color( Color( 1.0f, 0.5f, 0.25f ) );
                }else{
                    gl::color( Color::white() );
                }
                gl::draw( path );
            }
            void render(){
                vec2 pStart = output->getCenter(true);
                vec2 pStop = input->getCenter(true);
                vec2 pCenter = (pStart + pStop)*0.5f;
                vec2 p1 = vec2(pStart.x, pCenter.y);
                vec2 p2 = vec2(pStop.x, pCenter.y);
                path.moveTo( pStart );
                path.curveTo( p1, p2, pStop );
            }
        };//class Cable
        typedef shared_ptr<class Cable> CableRef;
    }//namespace UI {
}//namespace ORAGE {
#endif /* Cable_UI_h */
