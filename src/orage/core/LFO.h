#ifndef LFO_h
#define LFO_h


#include "Clock.h"
#include "Parameter.h"
#include "cinder/Rand.h"
#include "Module.h"
#include "Math.h"

class LFO : public Module {
	typedef std::shared_ptr<class LFO> LFORef;

	LFO();
	float time = 0;
	float rstTime = 0;
    
    void onResetChange(ParameterEventI event);
	public :

		ParameterRefI Mul;
		ParameterRefI Div;
		ParameterRefI Rst;
		ParameterRefI Rev;

		ParameterRefF Sin;
		ParameterRefF Rec;
		ParameterRefF Tri;
		ParameterRefF Saw;

		static LFORef create(){
            return LFORef( new LFO() );
        }
        virtual ~LFO();
        void update(ClockEvent event);
};

//////////////////////////////////////

using namespace std;

typedef shared_ptr<class LFO> LFORef;


LFO::LFO():
Module("LFO"){
	Mul = ParameterI::create(1, 1, 16);
	Div = ParameterI::create(1, 1, 16);
	Rst = ParameterI::create(0, 0, 1);
    
    Rst->getChangeSignal()->connect(boost::bind(&LFO::onResetChange, this, _1));
	Rev = ParameterI::create(0, 0, 1);

	Sin = ParameterF::create(0, 0, 1);
	Rec = ParameterF::create(0, 0, 1);
	Tri = ParameterF::create(0, 0, 1);
	Saw = ParameterF::create(0, 0, 1);
}

LFO::~LFO(){
    Rst->getChangeSignal()->disconnect_all_slots();
}

void LFO::update(ClockEvent event){
	time = event.time;
	float t = (time - rstTime) * Mul->getValue() / Div->getValue();
	float saw = t - floor(t);
	if(Rev->getValue() == 1){
		saw = 1 - saw;
	}
	Sin->setValue((cos(saw * M_PIl * 2)+1)*0.5); 
    Rec->setValue(saw > .5 ? 0 : 1);
	Tri->setValue(abs(1-(saw * 2))); 
	Saw->setValue(saw); 
}

void LFO::onResetChange(ParameterEventI event){
    if(event.value == 1){
        rstTime = time;
    }
}


#endif /* LFO_h */
