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

namespace ORAGE {
    namespace COMPONENTS {
        using namespace ci;
        using namespace ci::gl;
        using namespace ci::app;
        using namespace std;

        class ModuleController : public Module{
            typedef shared_ptr<ModuleController> ModuleControllerRef;
            string source;
            duk_context *ctx;
            DukValue testObj;
            char * _path;
        public :
            DukValue push_file_as_string(const char *filename) {
                std::ifstream t(filename);
                std::stringstream buffer;
                buffer << t.rdbuf();
                std::string s = buffer.str();
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
                Module(name), _path(&path[0])
            {
                moduleType = TYPES::CONTROLLER;
                UI->setColorBack(Config::getConfig(moduleType).bgColor);
                
                ctx = duk_create_heap_default();
                duk_push_c_function(ctx, native_print, DUK_VARARGS);
                duk_put_global_string(ctx, "print");
                testObj = push_file_as_string(_path);
                string value = dukglue_pcall_method<string>(ctx, testObj, "getConf", NULL);
                cout<<value<<endl;
            }
            virtual ~ModuleController(){
                //duk_destroy_heap(ctx);
            }
            static ModuleControllerRef create(string name, string path){
                return ModuleControllerRef(new ModuleController(name, path));
            }
            void updateSource(string * src){
                string tokenT = "${TIME}";
                string tokenDT = "${TIMEDELTA}";
                string tokenFI = "${FRAMEINDEX}";
                auto posT = src->find(tokenT);
                if(posT != string::npos)
                    src->replace(posT,tokenT.length(), std::to_string(getValue("TIME")->currentVal().getDoubleVal()));
                auto posDT = src->find(tokenDT);
                 if(posDT != string::npos)
                     src->replace(posDT,tokenDT.length(), std::to_string(getValue("TIMEDELTA")->currentVal().getDoubleVal()));
                auto posFI = src->find(tokenFI);
                if(posFI != string::npos)
                    src->replace(posFI,tokenFI.length(), std::to_string(getValue("FRAMEINDEX")->currentVal().getLongVal()));
            }
            virtual void draw() override {
                Module::draw();
                //string src = source;
                //updateSource(&src);
                //duk_eval_string(ctx, &src[0]);
                //https://github.com/Aloshi/dukglue
                string value = dukglue_pcall_method<string>(ctx, testObj, "main", getValue("TIME")->currentVal().getDoubleVal());
                cout<<value<<endl;
            }
        };
        typedef shared_ptr<ModuleController> ModuleControllerRef;
    }
}


#endif /* ModuleController_h */
