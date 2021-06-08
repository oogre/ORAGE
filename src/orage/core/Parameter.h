#ifndef Parameter_h
#define Parameter_h

#include "Module.h"
#include "Math.h"
#include <boost/signals2.hpp>

template<class T>
class Parameter;

template <typename T>
struct ParameterEvent{
    std::shared_ptr<class Parameter<T>> target;
    T value;
    float n_value;
    std::time_t timeStamp;
    ParameterEvent(std::shared_ptr<class Parameter<T>> target, T value) :
        target(target),
        timeStamp(std::time(0)),
        value(value),
        n_value((value - target->getMin()) * target->getRatio())
    {
    };
};

//////////////////////////////////////

template<class T>
class Parameter : public Module{
    typedef boost::signals2::signal<void(ParameterEvent<T>)> ParameterEventSignal;
    ParameterEventSignal parameterSignal;
    void eventTrigger(T value);
    bool onVerobse(ParameterEvent<T> event);
    float _ratio;
    T _val;
    T _min;
    T _max;
    Parameter(T v, T m, T M);
    T limiter(T v, T min, T max);
public :
    static std::shared_ptr<class Parameter<T>> create(T v, T m, T M){
        return std::shared_ptr<class Parameter<T>>( new Parameter(v, m, M) );
    }
    virtual ~Parameter();
    void setValue(T v, bool forceEvent = false);
    void setNormalizedValue(float v, bool forceEvent = false);
    void setMin(T v);
    void setMax(T v);
    T getValue(bool normalized = false);
    T getMin();
    T getMax();
    float getRatio(){ return _ratio; }
    std::shared_ptr<class Parameter<T>> addEventListener(const typename ParameterEventSignal::slot_type slot);
    template<typename Callable>
    void removeEventListener(Callable slot){parameterSignal.disconnect(slot);}
};

//////////////////////////////////////

using namespace std;
using namespace boost::signals2;

typedef Parameter<int> ParameterI;
typedef Parameter<float> ParameterF;
typedef ParameterEvent<int> ParameterEventI;
typedef ParameterEvent<float> ParameterEventF;
typedef shared_ptr<class Parameter<int>> ParameterRefI;
typedef shared_ptr<class Parameter<float>> ParameterRefF;
typedef signal<void(ParameterEvent<int>)> ParameterEventSignalI;
typedef signal<void(ParameterEvent<float>)> ParameterEventSignalF;

template<class T>
Parameter<T>::Parameter(T v, T m, T M) :
    Module(),
    _val(v),
	_min(m),
	_max(M),
    _ratio(Math::reverseDiff<T>(m, M))
{
}

template<class T>
Parameter<T>::~Parameter(){
}

template<class T>
bool Parameter<T>::onVerobse(ParameterEvent<T> event){
    cout<<event.target->getName(true)<<" : "<<event.value << " @ " << event.timeStamp <<endl;
    return true;
}

template<class T>
shared_ptr<class Parameter<T>> Parameter<T>::addEventListener(const typename ParameterEventSignal::slot_type slot){
    parameterSignal.connect(slot);
    return static_pointer_cast<Parameter<T>>(shared_from_this());
}

template<class T>
void Parameter<T>::eventTrigger(T value){
    ParameterEvent<T> event = {
        static_pointer_cast<Parameter<T>>(shared_from_this()),
        value
    };
    parameterSignal(event);
}

template<class T>
void Parameter<T>::setNormalizedValue(float v, bool forceEvent){
    if(strncmp(typeid(T).name(),"i",1) == 0){
        int valueCount = (getMax() - getMin());
        v = round(v * valueCount) / valueCount;
    }
    T _v = lerp(_min, _max, v);
    setValue(_v, forceEvent);
}

template<class T>
void Parameter<T>::setValue(T v, bool forceEvent){
    T _v = Math::constrain<T>(v, _min, _max);
    if(forceEvent || _val != _v){
        _val = _v;
        eventTrigger(_val);
    }
}

template<class T>
void Parameter<T>::setMin(T v){
    _min = v;
    _ratio = Math::reverseDiff<T>(_min, _max);
}

template<class T>
void Parameter<T>::setMax(T v){
    _max = v;
    _ratio = Math::reverseDiff<T>(_min, _max);
}

template<class T>
T Parameter<T>::getValue(bool normalized){
    if(!normalized) return _val;
    return (_val - _min) * _ratio;
}

template<class T>
T Parameter<T>::getMin(){
    return _min;
}

template<class T>
T Parameter<T>::getMax(){
    return _max;
}

#endif /* Parameter_h */
