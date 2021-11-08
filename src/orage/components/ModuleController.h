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
#include "json.hpp"
//https://duktape.org/
//https://github.com/Aloshi/dukglue
namespace ORAGE {
    namespace COMPONENTS {
        using namespace ci;
        using namespace ci::gl;
        using namespace ci::app;
        using namespace std;

        class ModuleController : public Module{
            typedef shared_ptr<ModuleController> ModuleControllerRef;
            duk_context *ctx;
            DukValue jsObject;
            JsonTree conf;
        public :
            DukValue push_file_as_string(const char *filename) {
                ifstream t(filename);
                stringstream buffer;
                buffer << t.rdbuf();
                string s = buffer.str();
                char* pString = new char[s.length() + 1];
                std::copy(s.c_str(), s.c_str() + s.length() + 1, pString);
                return dukglue_peval<DukValue>(ctx, pString);
            }
            static duk_ret_t native_print(duk_context *ctx) {
                duk_push_string(ctx, " ");
                duk_insert(ctx, 0);
                duk_join(ctx, duk_get_top(ctx) - 1);
                printf("%s\n", duk_to_string(ctx, -1));
                return 0;
            }
            
            ModuleController(string name, string path) :
                Module(name)
            {
                moduleType = TYPES::CONTROLLER;
                UI->setColorBack(Config::getConfig(moduleType).bgColor);
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
                        CustomISFAttrRef attr = addValue(name, "", "", ISFValType::ISFValType_Float, min, max, val);
                        UI->addParameter(name,
                                         attr->currentVal().getDoubleValPtr(),
                                         attr->minVal().getDoubleVal(),
                                         attr->maxVal().getDoubleVal(),
                                         ParameterFloat::Format().input(true));
                    }
                    if(input.getChild("TYPE").getValue() == "CLOCK"){
                        string name = input.getChild("NAME").getValue();
                        ISFVal TIMEDELTAmin (ISFValType::ISFValType_Float, 0.0);
                        ISFVal TIMEDELTAmax (ISFValType::ISFValType_Float, numeric_limits<double>::max());
                        ISFVal TIMEDELTAval (ISFValType::ISFValType_Float, 0.0);
                        CustomISFAttrRef attr = addValue(name, "", "", ISFValType::ISFValType_Float, TIMEDELTAmin, TIMEDELTAmax, TIMEDELTAval);
                        UI->addClock(name, attr, ParameterClock::Format().input(true));
                    }
                }
                for(auto output : conf.getChild("OUTPUTS").getChildren()){
                    if(output.getChild("TYPE").getValue() == "float"){
                        string name = output.getChild("NAME").getValue();
                        ISFVal min (ISFValType::ISFValType_Float, output.getChild("MIN").getValue<double>());
                        ISFVal max (ISFValType::ISFValType_Float, output.getChild("MAX").getValue<double>());
                        ISFVal val (ISFValType::ISFValType_Float, output.getChild("DEFAULT").getValue<double>());
                        CustomISFAttrRef attr = addValue(name, "", "", ISFValType::ISFValType_Float, min, max, val);
                        UI->addParameter(name,
                                         attr->currentVal().getDoubleValPtr(),
                                         attr->minVal().getDoubleVal(),
                                         attr->maxVal().getDoubleVal(),
                                         ParameterFloat::Format().input(false));
                    }
                    if(output.getChild("TYPE").getValue() == "CLOCK"){
                        string name = output.getChild("NAME").getValue();
                        ISFVal TIMEDELTAmin (ISFValType::ISFValType_Float, 0.0);
                        ISFVal TIMEDELTAmax (ISFValType::ISFValType_Float, numeric_limits<double>::max());
                        ISFVal TIMEDELTAval (ISFValType::ISFValType_Float, 0.0);
                        CustomISFAttrRef attr = addValue(name, "", "", ISFValType::ISFValType_Float, TIMEDELTAmin, TIMEDELTAmax, TIMEDELTAval);
                        UI->addClock(name, attr, ParameterClock::Format().input(false));
                    }
                }
                UI->autoSizeToFitSubviews();
            }
            virtual ~ModuleController(){
                //duk_destroy_heap(ctx);
            }
            static ModuleControllerRef create(string name, string path){
                return ModuleControllerRef(new ModuleController(name, path));
            }
            virtual void draw() override {
                Module::draw();
                //https://github.com/Aloshi/dukglue
                
                for(auto input : conf.getChild("INPUTS").getChildren()){
                    string name = input.getChild("NAME").getValue();
                    string type = input.getChild("TYPE").getValue();
                    if(type == "CLOCK"){
                        CustomISFAttrRef attr =  *(UI->parameters[name]->clockAttrIn);
                        dukglue_pcall_method<void>(ctx, jsObject, "setInput", name, attr->currentVal().getDoubleVal());
                    }else{
                        CustomISFAttrRef attr  = getValue(name);
                        dukglue_pcall_method<void>(ctx, jsObject, "setInput", name, attr->currentVal().getDoubleVal());
                    }
                }
                cout<<endl;
                
                string value = dukglue_pcall_method<string>(ctx, jsObject, "main",
                                                            getValue("TIME")->currentVal().getDoubleVal(),
                                                            getValue("TIMEDELTA")->currentVal().getDoubleVal(),
                                                            getValue("FRAMEINDEX")->currentVal().getDoubleVal()
                                                            );
                JsonTree outputs (value);
                for(auto output : outputs){
                    string name = output.getChild("NAME").getValue();
                    setValue(name, ISFVal(ISFValType::ISFValType_Float, output.getChild("VALUE").getValue<double>()));
                }
            }
        };
        typedef shared_ptr<ModuleController> ModuleControllerRef;
    }
}


#endif /* ModuleController_h */
