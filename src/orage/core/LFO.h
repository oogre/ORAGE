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
		ParameterRefI mul;
		ParameterRefI div;
		ParameterRefI rst;
		ParameterRefI rev;

		ParameterRefF sin;
		ParameterRefF rec;
		ParameterRefF tri;
        ParameterRefF saw;
        ParameterRefF noz;

		static LFORef create(){
            return LFORef( new LFO() );
        }
        virtual ~LFO();
        void run(ClockEvent event);
        virtual ModuleRef display(int x, int y, int w, int h) override;
        virtual void hide() override;
};

//////////////////////////////////////

using namespace std;

typedef shared_ptr<class LFO> LFORef;


LFO::LFO():
Module(){
    mul = addSubModule<ParameterI>("mul", ParameterI::create(1, 1, 16));
	div = addSubModule<ParameterI>("div", ParameterI::create(1, 1, 16));
	rst = addSubModule<ParameterI>("rst", ParameterI::create(0, 0, 1))
        ->addEventListener(boost::bind(&LFO::onResetChange, this, _1));
	rev = addSubModule<ParameterI>("rev", ParameterI::create(0, 0, 1));
    sin = addSubModule<ParameterF>("sin", ParameterF::create(0, 0, 1));
	rec = addSubModule<ParameterF>("rec", ParameterF::create(0, 0, 1));
	tri = addSubModule<ParameterF>("tri", ParameterF::create(0, 0, 1));
	saw = addSubModule<ParameterF>("saw", ParameterF::create(0, 0, 1));
    noz = addSubModule<ParameterF>("noz", ParameterF::create(0, 0, 1));;
    
//    noz->addEventListener([&](ParameterEvent<float> event)->void {
//        cout<<noz->to_string()<<endl;
//    });
}

LFO::~LFO(){
}

void LFO::run(ClockEvent event){
	time += event.timeUnit * mul->getValue() / div->getValue();
	float t = (time - rstTime);
	float _saw = t - floor(t);
	if(rev->getValue() == 1){
		_saw = 1 - _saw;
	}
	sin->setValue((cos(_saw * M_PIl * 2)+1)*0.5);
    rec->setValue(_saw > .5 ? 0 : 1);
	tri->setValue(abs(1-(_saw * 2)));
	saw->setValue(_saw);
}

void LFO::onResetChange(ParameterEventI event){
    if(event.value == 1){
        rstTime = time;
    }
}

ModuleRef LFO::display(int x, int y, int w, int h){
    Module::display(x, y, w, h);
    
    pannel->addSubView<UISliderI>("mul", UISliderI::create({10, 20 }, {size.x-20, 15}, mul));
    pannel->addSubView<UISliderI>("div", UISliderI::create({10, 50 }, {size.x-20, 15}, div));
    pannel->addSubView<UISliderI>("rst", UISliderI::create({10, 80 }, {size.x-20, 15}, rst));
    pannel->addSubView<UISliderI>("rev", UISliderI::create({10, 110}, {size.x-20, 15}, rev));
    pannel->addSubView<UISliderF>("sin", UISliderF::create({10, 140}, {size.x-20, 15}, sin));
    pannel->addSubView<UISliderF>("rec", UISliderF::create({10, 170}, {size.x-20, 15}, rec));
    pannel->addSubView<UISliderF>("tri", UISliderF::create({10, 200}, {size.x-20, 15}, tri));
    pannel->addSubView<UISliderF>("saw", UISliderF::create({10, 230}, {size.x-20, 15}, saw));
    pannel->addSubView<UISliderF>("noz", UISliderF::create({10, 260}, {size.x-20, 15}, noz));
    
    return shared_from_this();
}
void LFO::hide(){
    Module::hide();
}


#endif /* LFO_h */
