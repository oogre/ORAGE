//
//  OrageMenuButton.h
//  ORAGE
//
//  Created by Vincent Evrard on 24/10/21.
//

#ifndef OrageMenuButton_h
#define OrageMenuButton_h


#include "OrageMenuList.h"

namespace reza {
    namespace ui {
        using namespace ci;
        using namespace ci::app;
        using namespace std;
        class OrageMenuItem : public Button {
            typedef std::shared_ptr<class OrageMenuItem> OrageMenuItemRef;
            bool initialized = false;
            State oldState = State::NORMAL;
            std::function<void( ci::app::MouseEvent &event )> onMouseEnterFn;
            std::function<void( ci::app::MouseEvent &event )> onMouseLeaveFn;
        public :
            static OrageMenuItemRef create( std::string name, const Format &format = Format())
            {
                return OrageMenuItemRef( new OrageMenuItem( name, format));
            }
            
            OrageMenuItem( std::string name, const Format &format = Format()) :
            Button(name, false, format)
            {
            }
            
            ButtonRef addEntry(string name){
                if(!subMenu){
                    subMenu = OrageMenuList::create("");
                    subMenu->setOrigin(getOrigin() + vec2(0, getHeight() - 2));
                    onMouseEnter([&](ci::app::MouseEvent &event){
                        subMenu->setVisible(true);
                    });
                    onMouseLeave([&](ci::app::MouseEvent &event){
                        if( !subMenu->isHit( event.getPos() ) ) {
                            subMenu->setVisible(false);
                        }
                    });
                }
                Button::Format format = Button::Format().label(true).align(Alignment::CENTER).size(25);
                ButtonRef btn = Button::create(name, false, format);
                btn->setSize(vec2(100, 25));
                btn->setColorBack(ColorA(0, 0, 0, 0.0));
                btn->setColorBounds(ColorA(1, 1, 1, 1));
                subMenu->addSubViewDown(btn);
                subMenu->addSubViewToHeader(btn);
                btn->setVisible(false);
                return btn;
            }
            
            
            virtual void draw() override{
                Button::draw();
            }
            
            virtual ~OrageMenuItem(){
            }
            
            virtual void onMouseEnter( const std::function<void( ci::app::MouseEvent &event )> &callback ){
                onMouseEnterFn = callback;
            }
            virtual void onMouseLeave( const std::function<void( ci::app::MouseEvent &event )> &callback ){
                onMouseLeaveFn = callback;
            }
            virtual void mouseMove( ci::app::MouseEvent &event ) override {
                Button::mouseMove(event);
                State newState = getState();
                if(newState == State::OVER && oldState == State::NORMAL && onMouseEnterFn){
                    onMouseEnterFn(event);
                }
                if(newState == State::NORMAL && oldState == State::OVER && onMouseLeaveFn){
                    onMouseLeaveFn(event);
                }
                oldState = newState;
            }
            OrageMenuListRef subMenu;
            
        };//OrageMenuItem
        typedef std::shared_ptr<class OrageMenuItem> OrageMenuItemRef;
    }// ui
}// reza


#endif /* OrageMenuButton_h */