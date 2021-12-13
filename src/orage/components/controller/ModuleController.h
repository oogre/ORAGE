//
//  ModuleController.h
//  ORAGE
//
//  Created by Vincent Evrard on 27/10/21.
//

#ifndef ModuleController_h
#define ModuleController_h
#include "Module.h"
#include <dukglue/dukglue.h>

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
        
            void onError(DukErrorException & e){
                cerr << "ERROR FROM : " << UI->getName() << endl;
                cerr << e.what() << endl;
                UI->shouldDestroy = true;
            }
            
            DukValue push_file_as_string(const char *filename) {
                ifstream t(filename);
                stringstream buffer;
                buffer << t.rdbuf();
                string s = buffer.str();
                char* pString = new char[s.length() + 1];
                std::copy(s.c_str(), s.c_str() + s.length() + 1, pString);
                DukValue d = dukglue_peval<DukValue>(ctx, pString);
                delete[] pString;
                return d;
            }
            static duk_ret_t native_print(duk_context *ctx) {
                duk_push_string(ctx, " ");
                duk_insert(ctx, 0);
                duk_join(ctx, duk_get_top(ctx) - 1);
                printf("%s\n", duk_to_string(ctx, -1));
                return 0;
            }
            
            
        protected:
            virtual void UIReady() override {
                Module::UIReady();
                UI->setColorBack(Config::getConfig(moduleType).bgColor);
                for (auto& attr : _attributes->every()) {
                    if (attr->hasUI()) {
                        if(attr->isClock()){
                            UI->addClock(attr);
                        }
                        else if(attr->isFloat()){
                            UI->addLimitedSlider(attr);
                        }
                    }
                }
                UI->autoSizeToFitSubviews();
                
            }
        public :
            ModuleController(string name, string path, TYPES type) :
            Module(name)
            {
                moduleType = type;
                try{
                    ctx = duk_create_heap_default();
                    duk_push_c_function(ctx, native_print, DUK_VARARGS);
                    duk_put_global_string(ctx, "print");
                    jsObject = push_file_as_string(&path[0]);
                    string value = dukglue_pcall_method<string>(ctx, jsObject, "getConf", NULL);
                    conf = JsonTree(value);
                    
                    for(auto input : conf.getChild("INPUTS").getChildren()){
                        if(input.getChild("TYPE").getValue() == "float"){
                            string name = input.getChild("NAME").getValue();
                            ISFVal min (ISFValType::ISFValType_Float, input.getChild("MIN").getValue<float>());
                            ISFVal max (ISFValType::ISFValType_Float, input.getChild("MAX").getValue<float>());
                            ISFVal val (ISFValType::ISFValType_Float, input.getChild("DEFAULT").getValue<float>());
                            ISFAttrRef attr = _attributes->addAttr(ISFAttr::create(name, "", "", ISF::ISFAttr_IO::_IN, ISFValType::ISFValType_Float, min, max, val));
                            if(input.hasChild("MAGNETIC")){
                                vector<ISFVal> magnetic = vector<ISFVal>();
                                for(auto magnet : input.getChild("MAGNETIC").getChildren()){
                                    magnetic.push_back(ISFVal(ISFValType::ISFValType_Float, magnet.getValue<float>()));
                                }
                                attr->setMagnetic(magnetic);
                            }
                        }
                        if(input.getChild("TYPE").getValue() == "CLOCK"){
                            string name = input.getChild("NAME").getValue();
                            ISFVal TIMEDELTAmin (ISFValType::ISFValType_Float, 0.0);
                            ISFVal TIMEDELTAmax (ISFValType::ISFValType_Float, numeric_limits<double>::max());
                            ISFVal TIMEDELTAval (ISFValType::ISFValType_Float, 0.0);
                            _attributes->addAttr(ISFAttr::create(name, "", "", ISF::ISFAttr_IO::_IN, ISFValType::ISFValType_Clock, TIMEDELTAmin, TIMEDELTAmax, TIMEDELTAval));
                        }
                    }
                    for(auto output : conf.getChild("OUTPUTS").getChildren()){
                        if(output.getChild("TYPE").getValue() == "float"){
                            string name = output.getChild("NAME").getValue();
                            ISFVal min (ISFValType::ISFValType_Float, output.getChild("MIN").getValue<double>());
                            ISFVal max (ISFValType::ISFValType_Float, output.getChild("MAX").getValue<double>());
                            ISFVal val (ISFValType::ISFValType_Float, output.getChild("DEFAULT").getValue<double>());
                            _attributes->addAttr(ISFAttr::create(name, "", "", ISF::ISFAttr_IO::_OUT, ISFValType::ISFValType_Float, min, max, val));
                        }
                        if(output.getChild("TYPE").getValue() == "CLOCK"){
                            string name = output.getChild("NAME").getValue();
                            ISFVal TIMEDELTAmin (ISFValType::ISFValType_Float, 0.0);
                            ISFVal TIMEDELTAmax (ISFValType::ISFValType_Float, numeric_limits<double>::max());
                            ISFVal TIMEDELTAval (ISFValType::ISFValType_Float, 0.0);
                            ISFAttrRef attr = _attributes->addAttr(ISFAttr::create(name, "", "", ISF::ISFAttr_IO::_OUT, ISFValType::ISFValType_Clock, TIMEDELTAmin, TIMEDELTAmax, TIMEDELTAval));
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
            virtual void update() override {
                Module::update();
                //https://github.com/Aloshi/dukglue
                auto attrTime = _attributes->get("TIME");
                auto attrdTime = _attributes->get("TIMEDELTA");
                if(!!attrTime && attrdTime){
                    try{
                        for(auto input : conf.getChild("INPUTS").getChildren()){
                            string name = input.getChild("NAME").getValue();
                            
                            dukglue_pcall_method<void>(ctx, jsObject, "setInput", name, _attributes->get(name)->currentVal().getDoubleVal());
                        }
                        
                        string value = dukglue_pcall_method<string>(ctx, jsObject, "main",
                                                                   attrTime->currentVal().getDoubleVal(),
                                                                   attrdTime->currentVal().getDoubleVal());
                        JsonTree outputs (value);
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