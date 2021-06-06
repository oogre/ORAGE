#ifndef Parameter_h
#define Parameter_h

#include "Module.h"
#include "Math.h"
#include <boost/signals2.hpp>

template <typename T>
struct ParameterEvent{
    T value;
    float n_value;
};

//////////////////////////////////////

template<class T>
class Parameter : public Module{
    typedef boost::signals2::signal<void(ParameterEvent<T>)> ChangeSignal;
    private :
        float _ratio;
        T _val;
        T _min;
        T _max;
        Parameter(T v, T m, T M);
        T limiter(T v, T min, T max);
        ChangeSignal    changeSignal;
    public :
        static std::shared_ptr<class Parameter<T>> create(T v, T m, T M){
            return std::shared_ptr<class Parameter<T>>( new Parameter(v, m, M) );
        }
        virtual ~Parameter();
        void setValue(T v, bool forceEvent = false);
        void setMin(T v);
        void setMax(T v);
        T getValue(bool normalized = false);
        T getMin();
        T getMax();
        ChangeSignal* getChangeSignal();
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

template<class T>
Parameter<T>::Parameter(T v, T m, T M) :
    Module("Parameter"),
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
void Parameter<T>::setValue(T v, bool forceEvent){
    T _v = Math::constrain<T>(v, _min, _max);
    if(forceEvent || _val != _v){
        _val = _v;
        ParameterEvent<T> event = {
            _val,
            (_val - _min) * _ratio
        };
        changeSignal(event);
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
signal<void(ParameterEvent<T>)>* Parameter<T>::getChangeSignal(){
    return &changeSignal;
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
