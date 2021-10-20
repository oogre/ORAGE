//
//  Module.h
//  CinderProject
//
//  Created by Vincent Evrard on 6/10/21.
//

#ifndef Module_h
#define Module_h

#include "OrageCanvas.h"
#include "cinder/Rand.h"
#include "CustomISFAttr.h"
#include <ctime>
#include <limits>



namespace ORAGE {
    namespace COMPONENTS {
        using namespace ci;
        using namespace ci::gl;
        using namespace ci::app;
        using namespace std;
        using namespace reza::ui;
        using namespace VVISF;
        using namespace ORAGE::COMMON;
        
        class Module {
            typedef shared_ptr<Module> ModuleRef;
            static int ID;
            int id;
            
            float oldTime;
            float time;
            float dTime;
            vec4 date;
        public :
            map<string, CustomISFAttrRef> parameters;
            OrageCanvasRef UI;
            Module(string name){
                id = Module::ID++;
                time = oldTime  = getElapsedSeconds();
                
                ISFVal TIMEmin (ISFValType::ISFValType_Float, 0.0);
                ISFVal TIMEmax (ISFValType::ISFValType_Float, numeric_limits<float>::max());
                ISFVal TIMEval (ISFValType::ISFValType_Float, 0.0);
                parameters["TIME"] = CustomISFAttr::create("TIME", "", "", ISFValType::ISFValType_Float, TIMEmin, TIMEmax, TIMEval);
                
                ISFVal TIMEDELTAmin (ISFValType::ISFValType_Float, 0.0);
                ISFVal TIMEDELTAmax (ISFValType::ISFValType_Float, numeric_limits<float>::max());
                ISFVal TIMEDELTAval (ISFValType::ISFValType_Float, 0.0);
                parameters["TIMEDELTA"] = CustomISFAttr::create("TIMEDELTA", "", "", ISFValType::ISFValType_Float, TIMEDELTAmin, TIMEDELTAmax, TIMEDELTAval);
                
                UI = OrageCanvas::create( name + to_string(id) );
                UI->init();
            }
            
            Module * addEventListener(const string type, const typename boost::signals2::signal<void(ORAGE::COMMON::Event<ParameterBase>)>::slot_type slot) {
                for(auto [key, parameter] : UI->parameters){
                    parameter->addEventListener(type, slot);
                }
                return this;
            }
            void addValue(const std::string & inName,
                          const std::string & inDesc,
                          const std::string & inLabel,
                          const ISFValType & inType,
                          const ISFVal & inMinVal=ISFNullVal(),
                          const ISFVal & inMaxVal=ISFNullVal(),
                          const ISFVal & inDefVal=ISFNullVal(),
                          const ISFVal & inIdenVal=ISFNullVal(),
                          const std::vector<std::string> * inLabels=nullptr,
                          const std::vector<int32_t> * inVals=nullptr){
                parameters[inName] = CustomISFAttr::create(inName, inDesc, inLabel, inType, inMinVal, inMaxVal, inDefVal, inIdenVal, inLabels, inVals);
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
            
            void incValue(string name, int inc){
                setValue(name, ISFLongVal(parameters[name]->currentVal().getLongVal() + inc));
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
                
            }
            virtual void update(){
                time_t now = std::time(0);
                tm *ltm = localtime(&now);
                date = vec4(1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday, 0);
                time = getElapsedSeconds();
                setValue("TIME", ISFFloatVal(time));
                setValue("TIMEDELTA", ISFFloatVal(time - oldTime));
                oldTime = time;
                incValue("FRAMEINDEX", 1);
                UI->setNeedsDisplay();
            }
            bool hasToDestroy(){
                return UI->shouldDestroy;
            }
        };//Module {
        int Module::ID = 0;
        typedef shared_ptr<Module> ModuleRef;
    }//namespace COMPONENTS {
}//namespace ORAGE {

#endif /* Module_h */
