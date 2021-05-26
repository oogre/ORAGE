#ifndef Parameter_h
#define Parameter_h

#include "Module.h"

template<class T>
class Parameter : public Module{
	public :
		struct ParameterEvent{
			T value;
		};
	private :
		typedef std::function<void(ParameterEvent)> ParameterFnc;
		std::vector<ParameterFnc> parameterFncs;
		Parameter(T v, T m, T M) : 
			Module("Parameter"),
			value(v), 
			min(m), 
			max(M){	
		}
	public :
		static std::shared_ptr<class Parameter<T>> create(T v, T m, T M){
            return std::shared_ptr<class Parameter<T>>( new Parameter(v, m, M) );
        }
        
		void setValue(T v){
			T value = this->limiter(v, this->min, this->max);
			if(value != this->value){
				this->value = value;
				ParameterEvent event = {value}; 
				for(auto fnc : parameterFncs){
					fnc(event);
				}
			}
		}
		void setMin(T v){
			this->min = v;
		}
		void setMax(T v){
			this->max = v;
		}
		void onChanged(ParameterFnc fnc){
			parameterFncs.push_back(fnc);
		}
		T getValue(){
			return this->value;
		}
		T getMin(){
			return this->min;
		}
		T getMax(){
			return this->max;
		}
	private:
		T limiter(T v, T min, T max){
			v = v < min ? min : v;
			v = v > max ? max : v;
			return v;
		}
		T value;
		T min;
		T max;
};

//////////////////////////////////////

using namespace std;

typedef Parameter<int> ParameterI;
typedef Parameter<float> ParameterF;
typedef shared_ptr<class Parameter<int>> ParameterRefI;
typedef shared_ptr<class Parameter<float>> ParameterRefF;


#endif /* Parameter_h */