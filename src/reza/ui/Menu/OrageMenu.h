//
//  Menu.h
//  ORAGE
//
//  Created by Vincent Evrard on 24/10/21.
//

#ifndef Menu_h
#define Menu_h

#include "OrageMenuItem.h"
#include "EventTemplate.h"
#include "ModuleTypes.h"
namespace reza {
    namespace ui {
        using namespace ci;
        using namespace ci::app;
        using namespace std;
        using namespace reza::ui;
        using namespace ORAGE::COMPONENTS;
        using namespace ORAGE::COMMON;
        
        typedef EventTemplate<BaseEvent> BaseEventHandler;
        typedef boost::signals2::signal<void(BaseEvent)>::slot_type BaseEventSlot;
        
        typedef std::function<void(ORAGE::COMPONENTS::TYPES, vec2)> CALLBACK;
        typedef std::vector<std::pair<std::string, CALLBACK>> SUB_ENTRIES;
        
        class OrageMenu : public SuperCanvas, public BaseEventHandler{
            typedef shared_ptr<OrageMenu> OrageMenuRef;
            ci::signals::Connection resizeHandler;
            vector<OrageMenuItemRef> btns;
            virtual void enableUpdateCallback() override {/*DISABLE AUTO DRAWING AND UPDATE*/}
            
            OrageMenu(const WindowRef &window) :
                SuperCanvas("", window),
                BaseEventHandler()
            {
                disable();
                enable();
                resizeHandler = window->getSignalResize().connect(0 , [&](){
                    setSize(vec2(getWindowWidth(), 25));
                });
            }
        public :
            virtual ~OrageMenu(){
                resizeHandler.disconnect();
                for(auto btn : btns){
                    removeSubView(btn->getName());
                    btn->clear();
                }
                btns.clear();
                
            }
            static OrageMenuRef create(){
                return OrageMenuRef(new OrageMenu(ci::app::getWindow()));
            }
            
            void addElement(std::string name, SUB_ENTRIES subEntries){
                
                OrageMenuItemRef btn;
                for(OrageMenuItemRef _btn : btns){
                    if(_btn->getName() == name){
                        btn = _btn;
                        break;
                    }
                }
                
                if(!btn){
                    Button::Format format = Button::Format().label(true).align(Alignment::CENTER);
                    btn = OrageMenuItem::create(name, format);
                    btn->setSize(vec2(100, 25));
                    if(btns.size()==0) addSubView(btn);
                    else addSubViewEastOf(btn, btns.back()->getName());
                    addSubViewToHeader(btn);
                    btns.push_back(btn);
                }
                for (const auto & subEntry : subEntries){
                    string name = "";
                    string ext = "";
                    ORAGE::COMPONENTS::TYPES currentType = pathToComponentType(fs::path(subEntry.first), &name, &ext);
                    if(ORAGE::COMPONENTS::TYPES::NONE == currentType) continue;
                    btn->addEntry(name)->setCallback([&, subEntry, currentType, btn](bool a){
                        if(!a){
                            subEntry.second(currentType, btn->getOrigin());
                            btn->subMenu->setVisible(false);
                        }
                    });
                    ORAGE::COMPONENTS::Conf conf = ORAGE::COMPONENTS::Config::getConfig(currentType);
                    btn->setColorBack(conf.bgColor);
                    btn->subMenu->setColorBack(conf.bgColor);
                }
            }
            
            virtual void setup() override {
                SuperCanvas::setup();
                setColorBack(ColorA(1, 1, 1, 0.25));
                setColorBounds(ColorA(1, 1, 1, 1));
                setSize(vec2(getWindowWidth(), 25));
                ci::fs::path path;
                BaseEventHandler::eventTrigger({ "ready" });
            }
            
            virtual void draw() override {
                SuperCanvas::draw();
                if(isMinified()){
                    setMinified(false);
                }
            }
            virtual void mouseDrag( ci::app::MouseEvent &event ) override { }
            virtual void mouseDown( ci::app::MouseEvent &event ) override {
                Canvas::mouseDown( event );
            }
        };//OrageMenu
        typedef shared_ptr<OrageMenu> OrageMenuRef;
    }//MENU
}//ORAGE

#endif /* Menu_h */
