//
//  OrageMenuList.h
//  ORAGE
//
//  Created by Vincent Evrard on 26/10/21.
//

#ifndef OrageMenuList_h
#define OrageMenuList_h

#include "UI.h"
#include "cinder/Rand.h"
namespace reza {
    namespace ui {
        using namespace ci;
        using namespace ci::app;
        using namespace std;
        using namespace reza::ui;
        class OrageMenuList : public SuperCanvas{
            static int ID;
            Rand cRand;
            typedef shared_ptr<OrageMenuList> OrageMenuListRef;
            vector<ViewRef> btns;
        public :
            OrageMenuList(string name, const WindowRef &window) : SuperCanvas(name, window){
                cRand = Rand(ID++);
                onMouseEnter([&](ci::app::MouseEvent &event){
                    setVisible(true);
                });
                onMouseLeave([&](ci::app::MouseEvent &event){
                    setVisible(false);
                });
                setVisible(false);
            }
            virtual ~OrageMenuList(){
                for(auto btn : btns){
                    removeSubView(btn->getName());
                    btn->clear();
                }
                btns.clear();
            }
            static OrageMenuListRef create(string name){
                return std::make_shared<OrageMenuList>(name, ci::app::getWindow());
            }
            
            virtual void mouseDrag( ci::app::MouseEvent &event ) override{}
            virtual void mouseDown( ci::app::MouseEvent &event ) override{
                Canvas::mouseDown( event );
            }
            virtual void onMouseEnter( const std::function<void( ci::app::MouseEvent &event )> &callback ){
                onMouseEnterFn = callback;
            }
            virtual void onMouseLeave( const std::function<void( ci::app::MouseEvent &event )> &callback ){
                onMouseLeaveFn = callback;
            }
            virtual void mouseMove( ci::app::MouseEvent &event ) override {
                SuperCanvas::mouseMove(event);
                State newState ;
                if( isHit( event.getPos() ) ) {
                    newState = State::OVER;
                }
                else {
                    newState = State::NORMAL;
                }
                if(newState == State::OVER && oldState == State::NORMAL && onMouseEnterFn){
                    onMouseEnterFn(event);
                }
                if(newState == State::NORMAL && oldState == State::OVER && onMouseLeaveFn){
                    onMouseLeaveFn(event);
                }
                oldState = newState;
            }
            State oldState = State::NORMAL;
            std::function<void( ci::app::MouseEvent &event )> onMouseEnterFn;
            std::function<void( ci::app::MouseEvent &event )> onMouseLeaveFn;
        };//OrageMenuList
        int OrageMenuList::ID = 0;
        typedef shared_ptr<OrageMenuList> OrageMenuListRef;
    }//MENU
}//ORAGE


#endif /* OrageMenuList_h */
