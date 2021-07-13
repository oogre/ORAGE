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
        protected:
            Number(string name, string type = "Number") :
                Module(name, type)
            {
//                addEventListener("change", [](COMMON::Event<CORE::Module> event) -> void{
//                    cout<<event.target->getConf().dump()<<endl;
//                });
            };
        public :
            static NumberRef create(string name){
                return NumberRef( new Number( name ) );
            }
            virtual ~Number(){
            }
            virtual string to_string() override {
                return Module::to_string() + " : " + getStringValue();
            }
            virtual void setValue(float value) override {
                set<T>("value", (T)value);
            }
            virtual float getValue() override {
                return get<T>("value");
            }
            virtual string getStringValue() override {
                stringstream stream;
                stream << fixed << setprecision(2) << get<T>("value");
                return stream.str();
            }
            virtual void update(COMMON::Event<CORE::Module> event) override {
                
            }
            virtual void setConf(json conf) override {
                Module::setConf(conf);
                set<T>("value", conf["value"], true, false);
            }
        };//class Number<T>
        typedef Number<int> NumberI;
        typedef Number<float> NumberF;
        typedef shared_ptr<NumberI> NumberIRef;
        typedef shared_ptr<NumberF> NumberFRef;
    }//namespace CORE {
}//namespace ORAGE {
#endif /* Number_h */
