#ifndef Parameter_h
#define Parameter_h

#include "Math.h"
#include <boost/signals2.hpp>
#include "Module.h"

namespace ORAGE {
    namespace CORE {
        class Parameter;
        typedef std::shared_ptr<class Parameter> ParameterRef;
        using namespace std;
        using namespace boost::signals2;
        
        struct ParameterEvent{
            ParameterRef target;
            time_t timeStamp;
            string type;
            ParameterEvent(){}
            ParameterEvent(string type, ParameterRef target) :
                type(type),
                target(target),
                timeStamp(time(0))
            {
            };
            string to_string();
        };//struct ParameterEvent

        class Parameter : public Module{
        protected:
            typedef boost::signals2::signal<void(ParameterEvent)> ParameterEventSignal;
            typedef std::shared_ptr<class Parameter> ParameterRef;
            typedef std::map<string, ParameterEventSignal> MapType;
            ParameterEventSignal parameterSignal;
            MapType sigMap;

            Parameter(string name, string type = "Parameter") :
                Module(name, type)
            {
            }
        public :
            static ParameterRef create(std::string name){
                return ParameterRef( new Parameter( name ) );
            }
            virtual ~Parameter(){
            }
            virtual void addEventListener(const string type, const typename ParameterEventSignal::slot_type slot) override {
                if(sigMap.find(type) == sigMap.end()){
                    sigMap.insert(MapType::value_type({type, ParameterEventSignal()}));
                }
                sigMap.at(type).connect(slot);
            }
            template<typename Callable>
            void removeEventListener(const std::string type, Callable slot) {
                if(sigMap.find(type) != sigMap.end()){
                    sigMap.at(type).disconnect(slot);
                }
            }
            void eventTrigger(std::string type, ParameterRef target = nullptr){
                ParameterEvent event;
                if(target == nullptr){
                    event = ParameterEvent(type, as<Parameter>());
                }else{
                    event = ParameterEvent(type, target);
                }
                MapType::iterator signal = sigMap.find(type);
                if(signal == sigMap.end())return ;
                (sigMap.at(type))(event);
            }
            virtual ModuleRef addModule(ModuleRef module) override {
                auto t = Module::addModule(module);
                eventTrigger("add", module->as<Parameter>());
                return t;
            }
            virtual string getStringValue(){
                return "hello";
            }
            
        };//class Parameter
        typedef shared_ptr<class Parameter> ParameterRef;
        typedef signal<void(ParameterEvent)> ParameterEventSignal;
    }//namespace CORE
}//namespace ORAGE
#endif /* Parameter_h */
