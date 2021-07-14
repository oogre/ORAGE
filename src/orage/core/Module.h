#ifndef Module_h
#define Module_h

//#include "Cable.h"
//#include "../UI/Pannel.h"
#include "../../lib/Underscore.h"
#include "../../lib/Tools.h"
#include "../common/Node.h"
#include "../common/EventTemplate.h"
#include <boost/any.hpp>

namespace ORAGE {
    namespace CORE {
        using namespace std;
        using boost::any_cast;
        using json = nlohmann::json;
        
        class Module : public COMMON::Node, public COMMON::EventTemplate<Module> {
            
            typedef shared_ptr<class Module> ModuleRef;
        protected:
            json conf;
            unordered_map<string, boost::any> values;
            unordered_map<string, boost::any> defaultValues;
            Module(string name, string type = "Module") :
                Node(name, type),
                COMMON::EventTemplate<Module>()
            {
                
            }
        public:
            static ModuleRef create(string name){
                return ModuleRef( new Module(name) );
            }
            virtual ~Module(){
            }
            virtual void update(){
                forEach([&](COMMON::NodeRef node) -> void{
                    node->as<Module>()->update();
                });
            }
            virtual ModuleRef addModule(ModuleRef module){
                Node::addNode(module);
                eventTrigger({"add", module});
                return module;
            }
            virtual ModuleRef getModule(string name){
                auto temp = Node::getNode(name);
                if(temp != nullptr)return temp->as<Module>();
                throw invalid_argument( "getModule : unknown : " + name );
            }
            template<class T>
            void set(string key, T value, bool defaultValue = false, bool trigEvent= true){
                if(defaultValue){
                    defaultValues[key] = value;
                }
                values[key] = value;
                json::json_pointer jsonKey;
                jsonKey /= key;//append key
                conf[jsonKey] = value;
                if(trigEvent){
                    eventTrigger({"change", as<CORE::Module>()});
                }
            }
            template<class T>
            T get(string key){
                return any_cast<T>(values[key]);
            }
            virtual void reset(){
                values = defaultValues;
            }
            virtual void setValue(float value){
            }
            virtual float getValue(){
                return 0;
            }
            virtual string getStringValue(){
                return "";
            }
            virtual void setConf(json conf){
                this->conf = conf;
            }
            virtual json getConf(){
                return this->conf;
            }
            virtual void update(COMMON::Event<CORE::Module> event) {
                
            }
        };//class Module
        typedef shared_ptr<class Module> ModuleRef;
        
    }//namespace CORE
}//namespace ORAGE
#endif /* Module_h */
