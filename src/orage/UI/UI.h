//
//  OrageUI.h
//  Orage
//
//  Created by Vincent Evrard on 30/06/21.
//

#ifndef OrageUI_h
#define OrageUI_h

#include "Pannel.h"
#include "Number.h"
#include "Button.h"
namespace ORAGE {
    namespace UI {
        using namespace std;
        using json = nlohmann::json;
        
        enum class ViewType {
            ToggleButton,
            BangButton,
            Number
        };
        
        class Manager {
            ViewRef view;
            Manager(){
                view = IView::create("UI");
                view->setSize(getWindow()->getSize());
                view->setBgColor(Theme::bgColor);
                getWindow()->getSignalResize().connect([&]() -> void {
                    view->setSize(getWindow()->getSize());
                });
            }
            Manager(const Manager &old);
            const Manager &operator=(const Manager &old);
        public:
            static Manager &Instance(){
                static auto_ptr<Manager> instance( new Manager );
                return *instance;
            }
            virtual ~Manager(){
            }
            void update(){
                view->update();
            }
            void draw(){
                clear( Color( 0, 0, 0 ) );
                view->draw();
            }
            void addView(ORAGE::CORE::ParameterRef parameter){
                json conf = parameter->getConf();
                ViewType type = conf.at("/view"_json_pointer);
                string name = conf.at("/name"_json_pointer);
                
                ViewRef newView;
                switch(type){
                    case ViewType::Number :
                        newView = Number::create(name);
                        break;
                    case ViewType::BangButton :
                        newView = Button::create(name);
                        break;
                    case ViewType::ToggleButton :
                        newView = Button::create(name)->togglify();
                        break;
                    default :
                        newView = View::create(name);
                        break;
                }
                newView->setParameter(parameter);
                //newView->setParameter(parameter->getModule("bang")->as<ORAGE::CORE::Parameter>());
                view->addView( newView );
            }
            template<typename T = View>
            shared_ptr<T> getView(string name){
                return view->getView(name)->as<T>();
            }
            void addEventListener(const string type, const typename IViewEventSignal::slot_type slot){
                view->as<IView>()->addEventListener(type, slot);
            }
            string to_string(){
                return view->to_string();
            }
        };//class Manager{
    }//namespace UI {
}//namespace ORAGE {
#endif /* OrageUI_h */
