//
//  Module.h
//  CinderProject
//
//  Created by Vincent Evrard on 6/10/21.
//

#ifndef Module_h
#define Module_h

#include "OrageCanvas.h"
#include "CustomISFAttr.h"
#include "EventTemplate.h"
#include <ctime>
#include <limits>
#include <regex>

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
            
            double oldTime;
            double time;
            float dTime;
            vec4 date;
        public :
            TYPES moduleType;
            map<string, CustomISFAttrRef> parameters;
            OrageCanvasRef UI;
            Module(string name) :
                EvtModuleHandler()
            {
                auto index = name.find_last_of(".");
                if(index != std::string::npos){
                    name.erase( name.begin() + index, name.end());
                }
                moduleType = TYPES::NONE;
                id = Module::ID++;
                if(Module::IDS.count(name)==0){
                    Module::IDS[name] = 0;
                }else{
                    Module::IDS[name]++;
                }
                time = oldTime  = getElapsedSeconds();
                ISF::ISFAttr_IO io = ISF::ISFAttr_IO::_IN;
                ISFVal TIMEmin (ISFValType::ISFValType_Float, 0.0);
                ISFVal TIMEmax (ISFValType::ISFValType_Float, numeric_limits<double>::max());
                ISFVal TIMEval (ISFValType::ISFValType_Float, 0.0);
                parameters["TIME"] = CustomISFAttr::create("TIME", "", "", io, ISFValType::ISFValType_Float, TIMEmin, TIMEmax, TIMEval);
                
                ISFVal TIMEDELTAmin (ISFValType::ISFValType_Float, 0.0);
                ISFVal TIMEDELTAmax (ISFValType::ISFValType_Float, numeric_limits<double>::max());
                ISFVal TIMEDELTAval (ISFValType::ISFValType_Float, 0.0);
                parameters["TIMEDELTA"] = CustomISFAttr::create("TIMEDELTA", "", "", io, ISFValType::ISFValType_Float, TIMEDELTAmin, TIMEDELTAmax, TIMEDELTAval);
                
                ISFVal FRAMEINDEXmin(ISFValType::ISFValType_Float, 0.0);
                ISFVal FRAMEINDEXmax(ISFValType::ISFValType_Float, numeric_limits<double>::max());
                ISFVal FRAMEINDEXval(ISFValType::ISFValType_Float, 0.0);
                parameters["FRAMEINDEX"] = CustomISFAttr::create("FRAMEINDEX", "", "", io, ISFValType::ISFValType_Float, FRAMEINDEXmin, FRAMEINDEXmax, FRAMEINDEXval);
                
                UI = OrageCanvas::create( name + "." + to_string(Module::IDS[name]) );
                UI->init();
                UI->addEventListener("mouseDown", [&](EvtCanvas evt){
                    EvtModuleHandler::eventTrigger({
                        "putAtTop", shared_from_this()
                    });
                });
            }
            
            Module * addEventListenerOnParameters(const string type, EvtSlot slot) {
                for(auto [key, parameter] : UI->parameters){
                    parameter->addEventListener(type, slot);
                }
                return this;
            }
            
            
            CustomISFAttrRef addValue(const std::string & inName,
                                      const std::string & inDesc,
                                      const std::string & inLabel,
                                      const ISFAttr_IO io,
                                      const ISFValType & inType,
                                      const ISFVal & inMinVal=ISFNullVal(),
                                      const ISFVal & inMaxVal=ISFNullVal(),
                                      const ISFVal & inDefVal=ISFNullVal(),
                                      const ISFVal & inIdenVal=ISFNullVal(),
                                      const std::vector<std::string> * inLabels=nullptr,
                                      const std::vector<int32_t> * inVals=nullptr){
                parameters[inName] = CustomISFAttr::create(inName, inDesc, inLabel, io, inType, inMinVal, inMaxVal, inDefVal, inIdenVal, inLabels, inVals);
                return parameters[inName];
            }
            
            
            CustomISFAttrRef addValue(const ISFAttrRef attr){
                parameters[attr->name()] = CustomISFAttr::create(attr);
                return parameters[attr->name()];
            }
            
            bool hasValue(string name){
                return parameters.find(name) != parameters.end();
            }
            void setValue(string name, ISFVal value){
                parameters[name]->setCurrentVal(value);
            }
            
            CustomISFAttrRef getValue(string name){
                return parameters[name];
            }
            
            void incValue(string name, double inc){
                setValue(name, ISFFloatVal(parameters[name]->currentVal().getDoubleVal() + inc));
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
                setValue("TIME", ISFFloatVal(time));
                setValue("TIMEDELTA", ISFFloatVal(time - oldTime));
                oldTime = time;
                incValue("FRAMEINDEX", 1.0);
                UI->update();
            }
            bool hasToDestroy(){
                return UI->shouldDestroy;
            }
        };//Module {
        int Module::ID = 0;
        map<string, int> Module::IDS;
        typedef shared_ptr<Module> ModuleRef;
    }//namespace COMPONENTS {
}//namespace ORAGE {

#endif /* Module_h */
