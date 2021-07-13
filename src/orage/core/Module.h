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
            static int ID;
            typedef shared_ptr<class Module> ModuleRef;
        protected:
            unordered_map<string, boost::any> values;
            unordered_map<string, boost::any> defaultValues;
            Module(string name, string type = "Module") :
                Node(name, type),
                COMMON::EventTemplate<Module>()
            {
                id = Module::ID++;
            }
        public:
            int id ;
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
            template<typename T = Module>
            bool is(){
                return as<T>() != NULL;
            }
            virtual void set(string key, boost::any value, bool defaultValue = false){
                if(defaultValue){
                    defaultValues[key] = value;
                }
                values[key] = value;
            }
            virtual boost::any get(string key){
                return values[key];
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
        };//class Module
        typedef shared_ptr<class Module> ModuleRef;
        int Module::ID = 0;
    }//namespace CORE
}//namespace ORAGE
#endif /* Module_h */
