//
//  Module.h
//  CinderProject
//
//  Created by Vincent Evrard on 6/10/21.
//

#ifndef Module_h
#define Module_h

#include "OrageCanvas.h"
#include "ISFAttr.h"
#include "EventTemplate.h"
#include <ctime>
#include <limits>
#include <regex>
#include "ISFDoc.h"
#include "ISFAttrWrapper.h"

namespace ORAGE {
    namespace COMPONENTS {
        using namespace ci;
        using namespace ci::gl;
        using namespace ci::app;
        using namespace std;
        using namespace reza::ui;
        using namespace ISF;
        using namespace ORAGE::COMMON;
        
        typedef Event<class Module> EvtModule;
        typedef EventTemplate<EvtModule> EvtModuleHandler;
        typedef boost::signals2::signal<void(EvtModule)>::slot_type EvtModuleSlot;
        
        class Module : public EvtModuleHandler, public enable_shared_from_this<class Module> {
            typedef shared_ptr<Module> ModuleRef;
            static int ID;
            static map<string, int> IDS;
            int id;
            string _name;
            double oldTime;
            double time;
            float dTime;
            vec4 date;

        protected :
            virtual void UIReady() {
                UI->setColorBack(Config::getConfig(moduleType).bgColor);
            }
            
            Module(string name) :
            EvtModuleHandler()
            {
                
                moduleType = TYPES::NONE;
                id = Module::ID++;
                if(Module::IDS.count(name)==0){
                    Module::IDS[name] = 0;
                }else{
                    Module::IDS[name]++;
                }
                
                auto index = name.find_last_of(".");
                if(index != std::string::npos){
                    name.erase( name.begin() + index, name.end());
                }
                _name = name + "." + to_string(Module::IDS[name]);
                
                time = oldTime  = getElapsedSeconds();
                _attributes = ISFAttrWrapper::create();
                
                ISF::ISFAttr_IO io = ISF::ISFAttr_IO::_IN;
                ISFVal TIMEmin (ISFValType::ISFValType_Float, 0.0);
                ISFVal TIMEmax (ISFValType::ISFValType_Float, numeric_limits<double>::max());
                ISFVal TIMEval (ISFValType::ISFValType_Float, 0.0);
                ISFAttrRef time = _attributes->addAttr(ISFAttr::create("TIME", "", "", io, ISFValType::ISFValType_Float, TIMEmin, TIMEmax, TIMEval));
                time->disableUI();
                
                ISFVal TIMEDELTAmin (ISFValType::ISFValType_Float, 0.0);
                ISFVal TIMEDELTAmax (ISFValType::ISFValType_Float, numeric_limits<double>::max());
                ISFVal TIMEDELTAval (ISFValType::ISFValType_Float, 0.0);
                ISFAttrRef timedelta = _attributes->addAttr(ISFAttr::create("TIMEDELTA", "", "", io, ISFValType::ISFValType_Float, TIMEDELTAmin, TIMEDELTAmax, TIMEDELTAval));
                timedelta->disableUI();
                
                UI = OrageCanvas::create( _name );
                
                UI->addEventListener([&](EvtCanvas evt){
                    if(evt.is("mouseDown")){
                        EvtModuleHandler::eventTrigger({
                            "putAtTop", shared_from_this()
                        });
                    }
                    
                    if (evt.is("ready")) {
                        UIReady();
                        EvtModuleHandler::eventTrigger({
                            "ready", shared_from_this()
                        });
                    }
                });
            }
        public :
            TYPES moduleType;
            OrageCanvasRef UI;
            

            Module * addEventListenerOnParameters(EvtSlot slot) {
                for(auto [key, parameter] : UI->getParameters()){
                    parameter->addEventListener(slot);
                }
                return this;
            }
            
            virtual ~Module(){
                this->UI->clear();
            }
            static ModuleRef create(string name){
                return ModuleRef(new Module(name));
            }
            void setOrigin(vec2 pos){
                UI->setOrigin(pos);
            }
            virtual void draw(){
                UI->draw();
            }
            virtual void update(){
                time_t now = std::time(0);
                tm *ltm = localtime(&now);
                date = vec4(1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday, 0);
                time = getElapsedSeconds();
                
                _attributes->get("TIME")->setCurrent(time);
                _attributes->get("TIMEDELTA")->setCurrent(time - oldTime);
                
                oldTime = time;
                UI->update();
            }
            bool hasToDestroy(){
                return UI->shouldDestroy;
            }
            ISFAttrWrapperRef & attributes(){ return _attributes; }
            
            string name(){ return _name; }
        protected :
            ISFAttrWrapperRef _attributes;
        };//Module {
        int Module::ID = 0;
        map<string, int> Module::IDS;
        typedef shared_ptr<Module> ModuleRef;
    }//namespace COMPONENTS {
}//namespace ORAGE {

#endif /* Module_h */
