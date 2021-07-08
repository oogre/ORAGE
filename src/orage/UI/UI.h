//
//  OrageUI.h
//  Orage
//
//  Created by Vincent Evrard on 30/06/21.
//

#ifndef OrageUI_h
#define OrageUI_h
#include "Cables.h"
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
            typedef COMMON::MouseEvent<View> MouseEvt;
            Manager(){
                cables = Cables::create();
                view = View::create("UI");
                view->setSize(getWindow()->getSize());
                view->setBgColor(Theme::bgColor);
                getWindow()->getSignalResize().connect([&]() -> void {
                    view->setSize(getWindow()->getSize());
                });
                view->addEventListener("plug", boost::bind(&Manager::onPlug, this, _1));
            }
            Manager(const Manager &old);
            const Manager &operator=(const Manager &old);
        public:
            ViewRef view;
            CablesRef cables;
            
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
                cables->draw();
            }
            void addCable(string inputName, string outputName){
                cables->addCable(getView(inputName)->as<Plug>(), getView(outputName)->as<Plug>());
            }
            void addView(ORAGE::CORE::ModuleRef module){
                json conf = module->getConf();
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
                newView->setModule(module);
                view->addView( newView );
            }
            template<typename T = View>
            shared_ptr<T> getView(string name){
                return view->getView(name)->as<T>();
            }
            string to_string(){
                return view->to_string();
            }
            bool onPlug(MouseEvt event){
                cout<<event.target->getName(true)<<endl;
                return true;
            }
        };//class Manager{
    }//namespace UI {
}//namespace ORAGE {
#endif /* OrageUI_h */
