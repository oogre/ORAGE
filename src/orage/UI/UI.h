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
            Number,
            Pannel
        };
        
        class Manager {
            typedef COMMON::MouseEvent<View> MouseEvt;
            typedef ORAGE::COMMON::Event<ORAGE::CORE::Module> ModuleEvt;
            Manager(){
                Manager::view->setSize(getWindow()->getSize());
                Manager::view->setBgColor(Theme::bgColor);
                getWindow()->getSignalResize().connect([&]() -> void {
                    Manager::view->setSize(getWindow()->getSize());
                });
                Manager::view->addEventListener("plug", boost::bind(&Manager::onPlug, this, _1));
            }
            Manager(const Manager &old);
            const Manager &operator=(const Manager &old);
        public:
            static ViewRef view;
            static CablesRef cables;
            
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
            void addCable(string inputName, string outputName, bool trigEvent = true){
                addCable(view->getView(inputName), view->getView(outputName), trigEvent);
            }
            void addCable(ViewRef input, ViewRef output, bool trigEvent = true){
                cables->addCable(input->getViews<PlugInput>()[0], output->getViews<PlugOutput>()[0], trigEvent);
            }
            
            void addCable(ORAGE::CORE::ModuleRef input, ORAGE::CORE::ModuleRef output, bool trigEvent = true){
                ViewRef inView;
                ViewRef outView;
                view->every<ORAGE::UI::View>([&](ORAGE::UI::ViewRef view) -> void {
                    if(view->getModule() && view->getModule()->id == input->id){
                        inView = view;
                    }
                    if(view->getModule() && view->getModule()->id == output->id){
                        outView = view;
                    }
                });
                addCable(inView, outView, trigEvent);
            }
            
            void addView(ORAGE::CORE::ModuleRef module, ViewRef base = view){
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
                    case ViewType::Pannel :
                        newView = Pannel::create(name);
                        break;
                    default :
                        newView = View::create(name);
                        break;
                }
                newView->setModule(module);
               
                if(newView->is<Pannel>()){
                    module->addEventListener("add", [newView, module](ModuleEvt event)->void{
                        json subConf = event.target->getConf();
                        if(subConf.contains("/display"_json_pointer) && subConf.at("/display"_json_pointer)){
                            Manager::Instance().addView(event.target, newView->getView("parameters"));
                        }
                    });
                }
                base->addView( newView );
            }
            template<typename T = View>
            shared_ptr<T> getView(string name){
                return view->getView(name)->as<T>();
            }
            string to_string(){
                return view->to_string();
            }
            bool onPlug(MouseEvt event){                
                if(event.target->is<ORAGE::UI::PlugInput>()){
                    cout<< "input : "<<view->getName(true)<<endl;
                    cables->addCable(event.target->as<ORAGE::UI::PlugInput>());
                }
                if(event.target->is<ORAGE::UI::PlugOutput>()){
                    cout<< "output : "<<view->getName(true)<<endl;
                    cables->addCable(event.target->as<ORAGE::UI::PlugOutput>());
                }
                return true;
            }
        };//class Manager{
        
        CablesRef Manager::cables = Cables::create();
        ViewRef Manager::view = View::create("UI");
    }//namespace UI {
}//namespace ORAGE {
#endif /* OrageUI_h */
