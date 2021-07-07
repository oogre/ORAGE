//
//  Number.h
//  Orage
//
//  Created by Vincent Evrard on 30/06/21.
//

#ifndef LimittedNumber_h
#define LimittedNumber_h

#include "Parameter.h"
#include "Number.h"
#include "Math.h"
#include "boost/variant.hpp"

template<class T>
class LimittedNumber : public Parameter {
    typedef std::shared_ptr<Number<T>> NumberRef;
    float ratio;
//    void swapMinMax(){
//        NumberRef t = maximum;
//        maximum = minimum;
//        minimum = t;
//    }
public:
    typedef std::shared_ptr<LimittedNumber> LimittedNumberRef;
    virtual void setConf(json conf) override;
protected:
    LimittedNumber(std::string name, T value, T minimum, T maximum);
public :
    static LimittedNumberRef create(std::string name, T value = 0, T minimum = 0, T maximum = 1){
        return LimittedNumberRef( new LimittedNumber( name, value, minimum, maximum ) );
    }
    virtual ~LimittedNumber();
    virtual std::string to_string(bool recursive = false) override;
    void setValue(T value);
    void setValueN(float value);
    void setMinimum(T value);
    void setMinimumN(float value);
    void setMaximum(T value);
    void setMaximumN(float value);
    float getValue(bool normalized = false);
    float getMinimum(bool normalized = false);
    float getMaximum(bool normalized = false);
    void reset();
};

//////////////////////////////////////

using namespace std;

typedef LimittedNumber<int> LimittedNumberI;
typedef LimittedNumber<float> LimittedNumberF;
typedef shared_ptr<LimittedNumberI> LimittedNumberIRef;
typedef shared_ptr<LimittedNumberF> LimittedNumberFRef;

template<class T>
LimittedNumber<T>::LimittedNumber(string name, T value, T minimum, T maximum) :
    Parameter(name)
    {
        addModule(Number<T>::create("val", value));
        addModule(Number<T>::create("min", minimum));
        addModule(Number<T>::create("max", maximum));
}

template<class T>
LimittedNumber<T>::~LimittedNumber(){
}

template<class T>
string LimittedNumber<T>::to_string(bool recursive){
    return Module::to_string(recursive) ;//+ " | value : " + std::to_string(value) + " - minimum : " + std::to_string(minimum) + " - maximum : " + std::to_string(maximum);
}

template<class T>
void LimittedNumber<T>::setConf(json conf){
    Parameter::setConf(conf);
    //setMinimum(conf.at("/limittedValue/minimum"_json_pointer));
    //setMaximum(conf.at("/limittedValue/maximum"_json_pointer));
    //setValue(conf.at("/limittedValue/value"_json_pointer));
    //eventTrigger("valuechange");
}

template<class T>
void LimittedNumber<T>::setValue(T value){
    //this->value = Math::constrain<T>(value, minimum, maximum);
}

template<class T>
void LimittedNumber<T>::setValueN(float value){
//    value = Math::constrain<float>(value, 0, 1);
//    if(strncmp(typeid(T).name(),"i",1) == 0){
//        int valueCount = (o_maximum - o_minimum);
//        value = round(value * valueCount) / valueCount;
//    }
//    value = ci::lerp(o_minimum, o_maximum, value);
//    setValue(value);
}

template<class T>
void LimittedNumber<T>::setMinimum(T value){
//    minimum = value;
//    if(minimum > maximum){
//        swapMinMax();
//    }
//    ratio = Math::reverseDiff<T>(minimum, maximum);
//    setValue(this->value);
}

template<class T>
void LimittedNumber<T>::setMinimumN(float value){
//    value = Math::constrain<float>(value, 0, 1);
//    value = ci::lerp(o_minimum, o_maximum, value);
//    setMinimum(value);
}

template<class T>
void LimittedNumber<T>::setMaximum(T value){
//    maximum = value;
//    if(minimum > maximum){
//        swapMinMax();
//    }
//    ratio = Math::reverseDiff<T>(minimum, maximum);
//    setValue(this->value);
}

template<class T>
void LimittedNumber<T>::setMaximumN(float value){
//    value = Math::constrain<float>(value, 0, 1);
//    value = ci::lerp(o_minimum, o_maximum, value);
//    setMaximum(value);
}

template<class T>
float LimittedNumber<T>::getValue(bool normalized){
//    if(!normalized) return value;
//    return (value - o_minimum) * ratio;
    return 0;
}

template<class T>
float LimittedNumber<T>::getMinimum(bool normalized){
//    if(!normalized) return minimum;
//    return (minimum - o_minimum) * ratio;
    return 0;
}

template<class T>
float LimittedNumber<T>::getMaximum(bool normalized){
//    if(!normalized) return maximum;
//    return (maximum - o_minimum) * ratio;
    return 0;
}

template<class T>
void LimittedNumber<T>::reset(){
//    setMinimum(o_minimum);
//    setMaximum(o_maximum);
//    setValue(o_value);
}


#endif /* LimittedNumber_h */
