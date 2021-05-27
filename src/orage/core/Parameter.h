#ifndef Parameter_h
#define Parameter_h

#include "Module.h"
#include "Math.h"


template<class T>
class Parameter : public Module{
    public :
        struct ParameterEvent{
            T value;
            float n_value;
        };
    private :
        typedef std::function<void(ParameterEvent)> ParameterFnc;
        std::vector<ParameterFnc> parameterFncs;
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
        void setMin(T v);
        void setMax(T v);
        T getValue();
        T getMin();
        T getMax();
        void onChanged(ParameterFnc fnc);
};

//////////////////////////////////////

using namespace std;

typedef Parameter<int> ParameterI;
typedef Parameter<float> ParameterF;
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
        ParameterEvent event = {
            _val,
            (_val - _min) * _ratio
        };
        for(auto fnc : parameterFncs){
            fnc(event);
        }
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
void Parameter<T>::onChanged(ParameterFnc fnc){
    parameterFncs.push_back(fnc);
}

template<class T>
T Parameter<T>::getValue(){
    return _val;
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
