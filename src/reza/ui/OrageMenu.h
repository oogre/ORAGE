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
        typedef ORAGE::COMMON::MenuEvent<fs::path> EvtMenu;
        typedef ORAGE::COMMON::EventTemplate<fs::path, EvtMenu> EvtMenuHandler;
        
        
        class OrageMenu : public SuperCanvas, public EvtMenuHandler{
            typedef shared_ptr<OrageMenu> OrageMenuRef;
            ci::signals::Connection resizeHandler;
            bool initialized = false;
            vector<ViewRef> btns;
            
            OrageMenu(const WindowRef &window) : SuperCanvas("", window), EvtMenuHandler(){
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
            void init(){
                setSize(vec2(getWindowWidth(), 25));
                setColorBack(ColorA(1, 1, 1, 0.25));
                setColorBounds(ColorA(1, 1, 1, 1));
                
                for (const auto & entry : fs::directory_iterator(getAssetPath("modules").string())){
                    if(entry.is_directory()){
                        Button::Format format = Button::Format().label(true).align(Alignment::CENTER);
                        OrageMenuItemRef btn = OrageMenuItem::create(entry.path().filename().string(), format);
                        btn->setSize(vec2(100, 25));
                        if(btns.size()==0) addSubView(btn);
                        else addSubViewEastOf(btn, btns.back()->getName());
                        addSubViewToHeader(btn);
                        btns.push_back(btn);
                        for (const auto & subEntry : fs::directory_iterator(entry.path())){
                            if(subEntry.is_regular_file()){
                                string name = subEntry.path().filename().string();
                                string ext = subEntry.path().extension().string();
                                name = name.substr(0, name.length() - ext.length());
                                TYPES currentType = TYPES::NONE;
                                if(ext == ".fs") currentType = TYPES::ISF;
                                else if(ext == ".js") currentType = TYPES::CONTROLLER;
                                if (currentType == TYPES::NONE) continue;
                                btn->addEntry(name)->setCallback([&, subEntry, currentType](bool a){
                                    if(a){
                                        EvtMenuHandler::eventTrigger({"menu", subEntry.path(), currentType});
                                    }
                                });
                                Conf conf = Config::getConfig(currentType);
                                btn->setColorBack(conf.bgColor);
                                btn->subMenu->setColorBack(conf.bgColor);
                            }
                        }
                    }
                }
                initialized = true;
            }
            virtual void draw() override {
                if(!initialized)init();
                SuperCanvas::draw();
                if(isMinified()){
                    setMinified(false);
                }
            }
            
            virtual void mouseDrag( ci::app::MouseEvent &event ) override{}
            virtual void mouseDown( ci::app::MouseEvent &event ) override{
                Canvas::mouseDown( event );
            }
        };//OrageMenu
        typedef shared_ptr<OrageMenu> OrageMenuRef;
    }//MENU
}//ORAGE

#endif /* Menu_h */
