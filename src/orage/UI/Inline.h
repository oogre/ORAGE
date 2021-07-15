//
//  Inline.h
//  Orage
//
//  Created by Vincent Evrard on 9/07/21.
//

#ifndef Inline_h
#define Inline_h
#include "View.h"

namespace ORAGE {
    namespace UI {
        using namespace std;
        using namespace ci;
        using namespace ci::gl;
        
        class Inline : public View {
            typedef std::shared_ptr<class Inline> InlineRef;
            vec2 positionAcc = {0, 0};
            vec2 margin = {5, 0};
        protected :
            Inline(string name, string type="Inline") :
                View(name, type)
            {
                setBgColor(Theme<Layout::Normal>::InlineBGColor);
            }
        public :
            static InlineRef create(string name){
                return InlineRef( new Inline(name) );
            }
            virtual ~Inline(){
            }
            virtual ORAGE::UI::ViewRef addView(ORAGE::UI::ViewRef view) override {
                view->setPos(positionAcc);
                view->setSize({getSize().y, getSize().y});
                positionAcc = positionAcc + margin + vec2(view->getSize().x, 0);
                return View::addView(view);
            }
        };//class Inline
        typedef std::shared_ptr<class Inline> InlineRef;
    }//namespace UI {
}//namespace ORAGE {
#endif /* Inline_h */
