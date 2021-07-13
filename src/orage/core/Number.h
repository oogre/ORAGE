//
//  Number.h
//  Orage
//
//  Created by Vincent Evrard on 30/06/21.
//

#ifndef Number_h
#define Number_h


#include "Module.h"
#include "boost/variant.hpp"




namespace ORAGE {
    namespace CORE {
        using namespace std;
        using json = nlohmann::json;
        
        template<class T>
        class Number : public Module {
            typedef shared_ptr<class Number<T>> NumberRef;
            T value;
            T o_value;
        protected:
            Number(string name, T value, string type = "Number") :
                Module(name, type)
            {
                o_value = this->value = value;
                set("value", value, true);
            };
        public :
            static NumberRef create(string name, T value = 0){
                return NumberRef( new Number( name, value ) );
            }
            virtual ~Number(){
            }
            virtual string to_string() override {
                return Module::to_string() + " : " + std::to_string(value);
            }
            virtual void setValue(float value) override {
                this->value = value;
                conf["/value"_json_pointer] = this->value;
                eventTrigger({"change", as<CORE::Module>()});
            }
            virtual float getValue() override {
                return value;
            }
            virtual void reset() override {
                setValue(o_value);
            }
            virtual void setConf(json conf) override {
                if(!conf.contains("/value"_json_pointer)){
                    throw invalid_argument( "ORAGE::CORE::Number::setConf needs /value");
                }
                if(!conf.contains("/defaultValue"_json_pointer)){
                    json data{{
                        "defaultValue",
                        conf.at("/value"_json_pointer)
                    }};
                    conf.insert(data.begin(), data.end());
                }
                value = conf.at("/value"_json_pointer);
                o_value = conf.at("/defaultValue"_json_pointer);
                Node::setConf(conf);
            }
            virtual string getStringValue() override {
                stringstream stream;
                stream << fixed << setprecision(2) << value;
                return stream.str();
            }
        };//class Number<T>
        typedef Number<int> NumberI;
        typedef Number<float> NumberF;
        typedef shared_ptr<NumberI> NumberIRef;
        typedef shared_ptr<NumberF> NumberFRef;
    }//namespace CORE {
}//namespace ORAGE {
#endif /* Number_h */
