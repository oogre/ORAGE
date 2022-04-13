//
//  ModuleController.h
//  ORAGE
//
//  Created by Vincent Evrard on 27/10/21.
//

#ifndef ModuleController_h
#define ModuleController_h
#include "Module.h"
#include "OrageJS.h"

//https://duktape.org/
//https://github.com/Aloshi/dukglue
namespace ORAGE {
    namespace COMPONENTS {
        using namespace ci;
        using namespace ci::gl;
        using namespace ci::app;
        using namespace std;
        using namespace ISF;
        
        class ModuleController : public Module{
            typedef shared_ptr<ModuleController> ModuleControllerRef;
            duk_context *ctx;
            DukValue jsObject;
            JsonTree conf;
            string rawContent;
            
            void onError(DukErrorException & e){
                cerr << "ERROR FROM : " << UI->getName() << endl;
                cerr << e.what() << endl;
                UI->shouldDestroy = true;
            }

        public :
            
            ModuleController(string name, string path, TYPES type) :
            Module(name)
            {
                moduleType = type;
                try{
                    ctx = duk_create_heap_default();

                    rawContent = ORAGE::COMMON::readFile(path);
                    jsObject = ORAGE::COMMON::JS::init(ctx, path);
                    conf = JsonTree(dukglue_pcall_method<string>(ctx, jsObject, "getConf"));
                    
                    
                    for(auto input : conf.getChild("INPUTS").getChildren()){
                        if(input.getChild("TYPE").getValue() == "float"){
                            ISFAttrRef attr = ISFAttr::create(  input.getChild("NAME").getValue(), "", "",
                                                                ISF::ISFAttr_IO::_IN,
                                                                ISFValType::ISFValType_Float,
                                                                ISFFloatVal(input.getChild("MIN").getValue<float>()),
                                                                ISFFloatVal(input.getChild("MAX").getValue<float>()),
                                                                ISFFloatVal(input.getChild("DEFAULT").getValue<float>())
                                                              );
                            if(input.hasChild("MAGNETIC")){
                                vector<ISFVal> magnetic = vector<ISFVal>();
                                for(auto magnet : input.getChild("MAGNETIC").getChildren()){
                                    magnetic.push_back(ISFFloatVal(magnet.getValue<float>()));
                                }
                                attr->setMagnetic(magnetic);
                            }
                            _attributes->addAttr(attr);
                        }
                        
                        if(input.getChild("TYPE").getValue() == "clock"){
                            _attributes->addAttr(
                                 ISFAttr::create(   input.getChild("NAME").getValue(), "", "",
                                                    ISF::ISFAttr_IO::_IN,
                                                    ISFValType::ISFValType_Clock,
                                                    ISFClockVal(0.0),
                                                    ISFClockVal(numeric_limits<double>::max()),
                                                    ISFClockVal(0.0)
                                                 )
                            );
                        }
                    }
                    for(auto output : conf.getChild("OUTPUTS").getChildren()){
                        if(output.getChild("TYPE").getValue() == "float"){
                            _attributes->addAttr(
                                ISFAttr::create( output.getChild("NAME").getValue(), "", "",
                                                 ISF::ISFAttr_IO::_OUT,
                                                 ISFValType::ISFValType_Float,
                                                 ISFFloatVal(output.getChild("MIN").getValue<double>()),
                                                 ISFFloatVal(output.getChild("MAX").getValue<double>()),
                                                 ISFFloatVal(output.getChild("DEFAULT").getValue<double>())
                                                )
                             );
                        }
                        if(output.getChild("TYPE").getValue() == "clock"){
                            _attributes->addAttr(
                                ISFAttr::create( output.getChild("NAME").getValue(), "", "",
                                                 ISF::ISFAttr_IO::_OUT, ISFValType::ISFValType_Clock,
                                                 ISFFloatVal(0.0),
                                                 ISFFloatVal(numeric_limits<double>::max()),
                                                 ISFFloatVal(0.0)
                                                )
                            );
                        }
                    }
                }catch(DukErrorException & e){
                    onError(e);
                }
            }
            virtual ~ModuleController(){
                //duk_destroy_heap(ctx);
                cout<<"~ModuleController"<<endl;
            }
            static ModuleControllerRef create(string name, string path, TYPES type = TYPES::CONTROLLER){
                return std::make_shared<ModuleController>(name, path, type);
            }
            
            virtual vec2 getOrigin(bool raw=false) override {
                if(raw){
                    try{
                        string rawPosition = dukglue_pcall_method<string>( ctx, jsObject, "getPosition");
                        ci::JsonTree origin = ci::JsonTree(rawPosition);
                        if(origin.hasChild("x") && origin.hasChild("x")){
                            return vec2(
                                        origin.getChild("x").getValue<float>(),
                                        origin.getChild("y").getValue<float>()
                                        );
                        }
                    }catch(DukErrorException & e){
                        onError(e);
                    }
                }
                return Module::getOrigin(raw);
            }
            
            virtual string serialize() {
                string r = "";
                try{
                    r = dukglue_pcall_method<string>( ctx, jsObject, "rebuild", rawContent, Module::serialize() );
                }catch(DukErrorException & e){
                    onError(e);
                }
                return r;
            }
            virtual void update() override {
                Module::update();
                
                //https://github.com/Aloshi/dukglue
                auto attrTime = _attributes->get("TIME");
                auto attrdTime = _attributes->get("TIMEDELTA");
                if(!!attrTime && attrdTime){
                    try{
                        for(auto attr : _attributes->inputs()){
                            dukglue_pcall_method<void>(
                                ctx,
                                jsObject,
                                "setInput",
                                attr->name(),
                                attr->currentVal().getDoubleVal()
                            );
                        }
                        
                        JsonTree outputs = JsonTree(
                            dukglue_pcall_method<string>(
                                ctx,
                                jsObject,
                                "main",
                                attrTime->currentVal().getDoubleVal(),
                                attrdTime->currentVal().getDoubleVal()
                            )
                        );
                        
                        for(auto output : outputs){
                            string name = output.getChild("NAME").getValue();
                            double value = output.getChild("VALUE").getValue<double>();
                            auto attr = _attributes->get(name);
                            attr->currentVal().setDoubleVal(value);
                            ParameterFloatRef attrUI = dynamic_pointer_cast<ParameterFloat>(UI->getParameter(attr->name()));
                            if(!!attrUI){
                                attrUI->sliderRef->setValue(value);
                            }
                            attr->eventTrigger({
                                "change",
                                attr
                            });
                        }
                        
                    }catch(DukErrorException & e){
                        onError(e);
                    }
                }
            }
        };
        typedef shared_ptr<ModuleController> ModuleControllerRef;
    }
}


#endif /* ModuleController_h */
