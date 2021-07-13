//
//  Plugable.h
//  Orage
//
//  Created by Vincent Evrard on 8/07/21.
//

#ifndef Plugable_h
#define Plugable_h

#include "Plug.h"
#include "View.h"

namespace ORAGE {
    namespace UI {
        using namespace std;
        using namespace ci;
        using namespace ci::gl;
        
        class Plugable : public View  {
            typedef std::shared_ptr<class Plugable> PlugableRef;
        protected :
            Plugable(string name, string type="Plugable") :
                View(name, type)
            {
                setBgColor({0, 0, 0, 0});
            }
        public :
            static PlugableRef create(string name){
                return PlugableRef( new Plugable(name) );
            }
            virtual ~Plugable(){
                
            }
            virtual void draw() override {
                View::draw();
            };
            void addPlug(string name){
                ORAGE::UI::ViewRef input = addView(Plug::create(name));
                input->setSize({5, 5});
                input->setPos({0, 0});
            }
            
        };//class Plugable
        typedef std::shared_ptr<class Plugable> PlugableRef;
    }//namespace UI {
}//namespace ORAGE {
#endif /* Plugable_h */
