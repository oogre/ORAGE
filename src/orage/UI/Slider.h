#include "../core/Parameter.h"


// We'll create a new Cinder Application by deriving from the App class.
class Slider {
	typedef std::shared_ptr<class Slider> SliderRef;
	Slider(float v, float m, float M) : 
		value(Parameter::create(v, 0, 1)), 
		min(Parameter::create(m, 0, 1)), 
		max(Parameter::create(M, 0, 1)){
			
		}
	public :
		virtual ~Slider(){}
		static SliderRef create(float v, float m, float M){
            return SliderRef( new Slider(v, m, M) );
        }
        
	public :
		ParameterRef value;
		ParameterRef min;
		ParameterRef max;

	private:
};
typedef std::shared_ptr<class Slider> SliderRef;