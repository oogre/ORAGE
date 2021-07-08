//
//  Cable.h
//  Orage
//
//  Created by Vincent Evrard on 8/07/21.
//

#ifndef Cable_UI_h
#define Cable_UI_h

#include "Plug.h"

namespace ORAGE {
    namespace UI {
        using namespace std;
        using namespace ci;
        using namespace ci::gl;
        class Cable  {
            typedef shared_ptr<class Cable> CableRef;
            Path2d  path;
        public :
            PlugRef input;
            PlugRef output;
        protected :
            Cable(PlugRef input, PlugRef output) :
                input(input),
                output(output)
            {
                path.moveTo( output->getPos(true) + (output->getSize() * 0.5f) );
                path.lineTo( input->getPos(true) + (output->getSize() * 0.5f) );
            }
        public :
            static CableRef create(PlugRef input, PlugRef output){
                return CableRef( new Cable(input, output) );
            }
            virtual ~Cable(){
                
            }
            virtual void draw() {
                gl::color( Color( 1.0f, 0.5f, 0.25f ) );
                gl::draw( path );
            };
        };//class Cable
        typedef shared_ptr<class Cable> CableRef;
    }//namespace UI {
}//namespace ORAGE {
#endif /* Cable_UI_h */
