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
            virtual void enableUpdateCallback() override {/*DISABLE AUTO DRAWING AND UPDATE*/}
            
            OrageMenu(const WindowRef &window) :
                SuperCanvas("", window),
                EvtMenuHandler()
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
            void init(){
                
                setColorBack(ColorA(1, 1, 1, 0.25));
                setColorBounds(ColorA(1, 1, 1, 1));
                
                
                map<string, fs::directory_entry> sorted_by_name;
                for (const auto & entry : fs::directory_iterator(getAssetPath("modules").string())){
                    sorted_by_name[entry.path().filename().string()] = entry;
                }
                
                for (const auto & [key, entry] : sorted_by_name){
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
                                ORAGE::COMPONENTS::TYPES currentType = ORAGE::COMPONENTS::TYPES::NONE;
                                if(name.find(".fx") != std::string::npos && ext == ".fs")
                                    currentType = ORAGE::COMPONENTS::TYPES::FX;
                                else if(name.find(".out") != std::string::npos && ext == ".fs")
                                    currentType = ORAGE::COMPONENTS::TYPES::OUTPUT;
                                else if(ext == ".fs")
                                    currentType = ORAGE::COMPONENTS::TYPES::ISF;
                                else if(name.find(".clk") != std::string::npos && ext == ".js")
                                    currentType = ORAGE::COMPONENTS::TYPES::CLOCK;
                                else if(name.find(".math") != std::string::npos && ext == ".js")
                                    currentType = ORAGE::COMPONENTS::TYPES::MATH;
                                else if(ext == ".js")
                                    currentType = ORAGE::COMPONENTS::TYPES::CONTROLLER;
                                else
                                    continue;
                                vec2 origin = btn->getOrigin();
                                
                                btn->addEntry(name)->setCallback([&, subEntry, currentType, origin, btn](bool a){
                                    if(!a){
                                        EvtMenuHandler::eventTrigger({"menu", subEntry.path(), currentType, origin});
                                        btn->subMenu->setVisible(false);
                                    }
                                });
                                ORAGE::COMPONENTS::Conf conf = ORAGE::COMPONENTS::Config::getConfig(currentType);
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
                setSize(vec2(getWindowWidth(), 25));
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
