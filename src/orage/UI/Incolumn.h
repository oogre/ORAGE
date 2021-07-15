//
//  Incolumn.h
//  Orage
//
//  Created by Vincent Evrard on 14/07/21.
//

#ifndef Incolumn_h
#define Incolumn_h

#include "View.h"

namespace ORAGE {
    namespace UI {
        using namespace std;
        using namespace ci;
        using namespace ci::gl;
        
        class Incolumn : public View {
            typedef std::shared_ptr<class Incolumn> IncolumnRef;
            vec2 positionAcc = {0, 0};
            vec2 margin = {0, 20};
        protected :
            Incolumn(string name, string type="Incolumn") :
                View(name, type)
            {
                setBgColor(Theme<Layout::Normal>::InlineBGColor);
            }
        public :
            static IncolumnRef create(string name){
                return IncolumnRef( new Incolumn(name) );
            }
            virtual ~Incolumn(){
            }
            virtual ORAGE::UI::ViewRef addView(ORAGE::UI::ViewRef view) override {
                view->setPos(positionAcc);
                view->setSize({getSize().x, view->getSize().y});
                positionAcc = positionAcc + margin + vec2(0, view->getSize().y);
                return View::addView(view);
            }
        };//class Incolumn
    }//namespace UI {
}//namespace ORAGE {

#endif /* Incolumn_h */
