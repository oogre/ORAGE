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
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.


namespace ORAGE {
    namespace COMPONENTS {
        using namespace ci;
        using namespace ci::gl;
        using namespace ci::app;
        using namespace std;
        using namespace reza::ui;
        using namespace ISF;
        using namespace ORAGE::COMMON;
        using namespace boost::uuids;
        
        typedef Event<class Module> EvtModule;
        typedef EventTemplate<EvtModule> EvtModuleHandler;
        typedef boost::signals2::signal<void(EvtModule)>::slot_type EvtModuleSlot;
        
        class Module : public EvtModuleHandler, public enable_shared_from_this<class Module> {
            typedef shared_ptr<Module> ModuleRef;
            string id;
            string _name;
            double oldTime;
            double time;
            float dTime;
            vec4 date;
            bool _ready = false;
            
        protected :
            EvtSlot parameterPlugHandler;
            
            virtual void UIReady() {
                UI->setColorBack(Config::getConfig(moduleType).bgColor);
                
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
            }
            
            Module(string nameId) :
                EvtModuleHandler(),
                parameterPlugHandler([&](Evt evt){})
            {
                int index = nameId.find_last_of(".");
                if (index != string::npos){
                    _name = nameId.substr(0, index);
                    id = nameId.substr(index+1);
                }else{
                    uuid uid = boost::uuids::random_generator()();
                    std::stringstream ss;
                    ss << uid;
                    _name = nameId;
                    id = ss.str();
                }
                
                moduleType = TYPES::NONE;
                
                time = oldTime  = getElapsedSeconds();
                _attributes = ISFAttrWrapper::create();
                
                UI = OrageCanvas::create( _name );
                
                UI->addEventListener([&](EvtCanvas evt){
                    if(evt.is("mouseDown")){
                        EvtModuleHandler::eventTrigger({
                            "putAtTop", shared_from_this()
                        });
                    }
                    
                    if (evt.is("ready")) {
                        UIReady();// building UI
                        EvtModuleHandler::eventTrigger({
                            "ready", shared_from_this()
                        });
                        _ready = true;
                        for (auto & inAttr : _attributes->imageInputs()) {
                            if(inAttr->name() == "OLD"){
                                UI->getParameter(inAttr->name())->eventTrigger({
                                    "plug", inAttr
                                });
                                auto firstOutAttr = _attributes->imageOutputs().begin();
                                UI->getParameter((*firstOutAttr)->name())->eventTrigger({
                                    "plug", (*firstOutAttr)
                                });
                            }
                        }
                        
                        for(auto [key, parameter] : UI->getParameters()){
                            parameter->addEventListener(parameterPlugHandler);
                        }
                    }
                });
            }
        public :
            TYPES moduleType;
            OrageCanvasRef UI;
            
            void onParameterPlug(EvtSlot slot){
                parameterPlugHandler = slot;
            }
//
//            Module * addEventListenerOnParameters(EvtSlot slot) {
//                for(auto [key, parameter] : UI->getParameters()){
//                    parameter->addEventListener(slot);
//                }
//                return this;
//            }
            bool isReady(){
                return _ready;
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
                
                auto attrTime = _attributes->get("TIME");
                auto attrdTime = _attributes->get("TIMEDELTA");
                if(!!attrTime && attrdTime){
                    attrTime->setCurrent(time);
                    attrdTime->setCurrent(time - oldTime);
                }
                oldTime = time;
                UI->update();
            }
            bool hasToDestroy(){
                return UI->shouldDestroy;
            }
            ISFAttrWrapperRef & attributes(){ return _attributes; }
            
            string name(bool withId = false){
                return _name + (withId ? ("." + id) : "");
            }
            
            string getId(){
                return id;
            }
            
            virtual string serialize() {
                ci::JsonTree tree = ci::JsonTree();
                ci::JsonTree inputs = ci::JsonTree::makeArray("INPUTS");
                for(auto attr : _attributes->inputs()){
                    ci::JsonTree input = ci::JsonTree();
                    
                    input.addChild(ci::JsonTree("NAME", attr->name()));
                    input.addChild(ci::JsonTree("TYPE", StringFromISFValType(attr->type())));
                    if(attr->type() != ISFValType_Image){
                        input.addChild(ci::JsonTree("DEFAULT", attr->currentVal().getDoubleVal()));
                        input.addChild(ci::JsonTree("MIN", attr->minVal().getDoubleVal()));
                        input.addChild(ci::JsonTree("MAX", attr->maxVal().getDoubleVal()));
                    }
                    inputs.addChild(input);
                }
                tree.addChild(inputs);
                ci::JsonTree outputs = ci::JsonTree::makeArray("OUTPUTS");
                for(auto attr : _attributes->outputs()){
                    ci::JsonTree output = ci::JsonTree();
                    output.addChild(ci::JsonTree("NAME", attr->name()));
                    output.addChild(ci::JsonTree("TYPE", StringFromISFValType(attr->type())));
                    if(attr->type() != ISFValType_Image){
                        output.addChild(ci::JsonTree("DEFAULT", attr->currentVal().getDoubleVal()));
                        output.addChild(ci::JsonTree("MIN", attr->minVal().getDoubleVal()));
                        output.addChild(ci::JsonTree("MAX", attr->maxVal().getDoubleVal()));
                    }
                    outputs.addChild(output);
                }
                tree.addChild(outputs);
                ci::JsonTree ui = ci::JsonTree::makeObject("UI");
                ci::JsonTree position = ci::JsonTree::makeObject("position");
                position.addChild(ci::JsonTree("x",  UI->getOrigin().x ));
                position.addChild(ci::JsonTree("y",  UI->getOrigin().y ));
                ui.addChild(ci::JsonTree("minified",  UI->isMinified() ));
                ui.addChild(position);
                tree.addChild(ui);
                return tree.serialize();
            }
            
            
                
        protected :
            ISFAttrWrapperRef _attributes;
        };//Module {
        typedef shared_ptr<Module> ModuleRef;
    }//namespace COMPONENTS {
}//namespace ORAGE {

#endif /* Module_h */
