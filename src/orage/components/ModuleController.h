//
//  ModuleController.h
//  ORAGE
//
//  Created by Vincent Evrard on 27/10/21.
//

#ifndef ModuleController_h
#define ModuleController_h
#include "Module.h"
#include "duktape.h"

namespace ORAGE {
    namespace COMPONENTS {
        using namespace ci;
        using namespace ci::gl;
        using namespace ci::app;
        using namespace std;

        class ModuleController : public Module{
            typedef shared_ptr<ModuleController> ModuleControllerRef;
            string source;
        public :
            ModuleController(string name, string path) :
                Module(name)
            {
                moduleType = TYPES::CONTROLLER;
                UI->setColorBack(Config::getConfig(moduleType).bgColor);
                
                std::ifstream t(path);
                std::stringstream buffer;
                buffer << t.rdbuf();
                source = buffer.str();
            }
            virtual ~ModuleController(){}
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
                string src = source;
                updateSource(&src);
                duk_context *ctx = duk_create_heap_default();
                duk_eval_string(ctx, &src[0]);
                cout<<duk_get_string(ctx, -1)<<endl;;
                duk_destroy_heap(ctx);
            }
        };
        typedef shared_ptr<ModuleController> ModuleControllerRef;
    }
}


#endif /* ModuleController_h */
