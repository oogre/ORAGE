#ifndef Module_h
#define Module_h

//#include "Cable.h"
//#include "../UI/Pannel.h"
#include "../../lib/Underscore.h"
#include "../../lib/Tools.h"
#include "../common/Node.h"

namespace ORAGE {
    namespace CORE {
        using namespace std;
        using json = nlohmann::json;
        
        struct ParameterEvent;
        
        class Module : public COMMON::Node{
            static int ID;
            typedef boost::signals2::signal<void(ParameterEvent)> ParameterEventSignal;
            
        protected:
            Module(string name, string type = "Module") :
                Node(name, type)
            {
                id = Module::ID++;
            }
        public:
            int id ;
            virtual shared_ptr<class Module> addModule(shared_ptr<class Module> module){
                return Node::addNode(module)->as<Module>();
            }
            virtual shared_ptr<class Module> getModule(string name){
                auto temp = Node::getNode(name);
                if(temp != nullptr)return temp->as<Module>();
                throw invalid_argument( "getModule : unknown : " + name );
            }
            static shared_ptr<class Module> create(string name){
                return shared_ptr<class Module>( new Module(name) );
            }
           
            virtual ~Module(){
            }
            virtual void update(){
                forEach([&](COMMON::NodeRef node) -> void{
                    node->as<Module>()->update();
                });
            }
            template<typename T = Module>
            bool is(){
                return as<T>() != NULL;
            }
            virtual void setValue(float value){
                
            };
            virtual float getValue(){
                return 0;
            };
            virtual void addEventListener(const string type, const typename ParameterEventSignal::slot_type slot){
            }
        };//class Module
        typedef shared_ptr<class Module> ModuleRef;
        int Module::ID = 0;
    }//namespace CORE
}//namespace ORAGE
#endif /* Module_h */
