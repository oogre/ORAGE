#ifndef Clock_h
#define Clock_h

#include "../../lib/chrono_io.h"
#include "../UI/Clock.h"
#include "Parameter.h"
#include "Math.h"
#include "Module.h"

struct ClockEvent{
	float time;
	float progress;
	Math::FRACTION_LIST list;
	bool is(int num, int den);
};

//////////////////////////////////////

class Clock : public Module{
	typedef std::shared_ptr<class Clock> ClockRef;
	typedef std::function<void(ClockEvent)> StrobFnc;

	bool mShouldQuit;
	shared_ptr<thread> mThread;

	static const uint16_t StepPerBeat = 840;
	static constexpr const float subStepInterval = 1.0f/StepPerBeat;
	static Math::LISTOF_FRACTION_LIST FRACTIONS;

	float BPM;
	double interval;
	ParameterRefI bang;
	std::vector<StrobFnc> strobFuncs;

	void update();
	void trigStrob(ClockEvent event);
	Clock(float BPM = 60);
	public :
		static ClockRef create(float BPM = 60){
            return ClockRef( new Clock(BPM) );
        }
		virtual ~Clock() override;
		void setBPM(float BPM);
		float getBPM();
		void strob(StrobFnc fnc);
		virtual ModuleRef display(int x, int y, int w, int h) override;
};

//////////////////////////////////////

Math::LISTOF_FRACTION_LIST Clock::FRACTIONS = Math::getFractions(StepPerBeat);

using namespace ci;
using namespace std;
using namespace std::chrono;

typedef shared_ptr<class Clock> ClockRef;
typedef function<void(ClockEvent)> StrobFnc;

Clock::Clock(float BPM):
Module("Clock"){
	this->setBPM(BPM);
	bang = ParameterI::create(1, 0, 1);
	this->strob([&](ClockEvent event) -> void{
		if(event.is(1, 1)){
			bang->setValue(1);
		}else if(event.is(1, 2)){
			bang->setValue(0);
		}
	});
	mShouldQuit = false;
	mThread = shared_ptr<thread>(new thread(bind(&Clock::update, this)));
}

Clock::~Clock(){
	mShouldQuit = true;
	Module::~Module();
}

void Clock::update(){
	typedef duration<double> DURATION;
	typedef high_resolution_clock CLCK;

	ThreadSetup threadSetup;
	double delay = 0;
	int counter = 0;
	auto t0 = CLCK::now();
	auto t1 = t0;
	while(!mShouldQuit){
		auto t2 = CLCK::now();
		float time =  duration_cast<DURATION>(t2 - t0).count();
		delay += duration_cast<DURATION>(t2 - t1).count();
		if(delay >= interval){
			delay -= interval;
			trigStrob({
				time, 
				(++counter) * subStepInterval, 
				Clock::FRACTIONS[counter-1]
			});
			if(counter >= StepPerBeat){
				counter = 0;
			}
		}
		t1 = t2;
	}
}

void Clock::setBPM(float BPM){
	this->BPM = BPM;
	interval = 60 / (double)(BPM * StepPerBeat);
}

float Clock::getBPM(){
	return this->BPM;
}

void Clock::trigStrob(ClockEvent event){
	for(auto fnc : strobFuncs){
		fnc(event);
	}
}

void Clock::strob(StrobFnc fnc){
	strobFuncs.push_back(fnc);
}

ModuleRef Clock::display(int x, int y, int w, int h){
	view = UIClock::create({x, y}, {w, h});
	
	bang->onChanged([&](ParameterI::ParameterEvent event) -> void{
		if(event.value == 1){
			view->getSubView("bang")->bgColor = Theme::bgActiveColor;
		}
		if(event.value == 0){
			view->getSubView("bang")->bgColor = Theme::bgDisactiveColor;
		}
	});

	return shared_from_this();
}

//////////////////////////////////////

bool ClockEvent::is(int num, int den){
	return find(list.begin(), list.end(), make_pair(num, den)) != list.end();
}

#endif /* Clock_h */