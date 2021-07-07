//
//  Button.h
//  Orage
//
//  Created by Vincent Evrard on 7/07/21.
//

#ifndef Button_h
#define Button_h

#include "IView.h"
/*
 toggle
 bang
*/
namespace ORAGE {
    namespace UI {
        using namespace std;
        using namespace ci;
        using namespace ci::gl;
        using json = nlohmann::json;
        
        class Button : public IView {
            typedef shared_ptr<class Button> ButtonRef;
            enum ButtonType {
                BANG,
                TOGGLE
            };
            ButtonType type = BANG;
            Button(string name, string type="Button") :
            IView(name, type)
            {
                setSize(Theme::ButtonSize);
                addEventListener("down", boost::bind(&Button::onDown, this, _1));
                addEventListener("up", boost::bind(&Button::onUp, this, _1));
                setBgColor(Theme::bgDisactiveColor);
            }
            bool onDown(IViewEvent event){
                switch(type){
                    case BANG :
                        turnOn();
                        break;
                    case TOGGLE :
                        break;
                }
                return true;
            }
            bool onUp(IViewEvent event){
                switch(type){
                    case BANG :
                        turnOff();
                        break;
                    case TOGGLE :
                        toggle();
                        break;
                }
                
                return true;
            }
        public :
            void turnOn(){
                parameter->setValue(parameter->getConf().at("on"));
            }
            void turnOff(){
                parameter->setValue(parameter->getConf().at("off"));
            }
            void toggle(){
                if(isOn()){
                    turnOff();
                }else{
                    turnOn();
                }
            }
            bool isOn(){
                return parameter->getValue() == parameter->getConf().at("on");
            }
            static ButtonRef create(string name){
                return ButtonRef( new Button(name) );
            }
            ButtonRef togglify(){
                type = TOGGLE;
                return as<Button>();
            }
            virtual ~Button() {
                removeEventListener("down", boost::bind(&Button::onDown, this, _1));
                removeEventListener("up", boost::bind(&Button::onUp, this, _1));
                parameter->removeEventListener("change", boost::bind(&Button::onChange, this, _1));
            }
            virtual void draw() override {
                pushModelView();
                IView::draw();
                popModelView();
            }
            virtual void setParameter(CORE::ParameterRef parameter) override {
                View::setParameter(parameter);
                parameter->addEventListener("change", boost::bind(&Button::onChange, this, _1));
                render();
            }
            void onChange(CORE::ParameterEvent event){
                render();
            }
            void render(){
                if(isOn()){
                    setBgColor(Theme::bgActiveColor);
                }else{
                    setBgColor(Theme::bgDisactiveColor);
                }
            }
        };//class Number : public IView {
        typedef shared_ptr<class Number> NumberRef;
    }//namespace UI {
}//namespace ORAGE {

#endif /* Button_h */
