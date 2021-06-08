#ifndef Clock_h
#define Clock_h

#include "../../lib/chrono_io.h"
#include "../UI/IView.h"
#include "../UI/Slider.h"
#include "../UI/Pannel.h"
#include "Parameter.h"
#include "Math.h"
#include "Module.h"
#include <boost/signals2.hpp>

struct ClockEvent{
	float timeUnit;
	float progress;
	Math::FRACTION_LIST list;
	bool is(int num, int den);
};

//////////////////////////////////////

bool ClockEvent::is(int num, int den){
    return find(list.begin(), list.end(), make_pair(num, den)) != list.end();
}

//////////////////////////////////////

class Clock : public Module{
	typedef std::shared_ptr<class Clock> ClockRef;
    typedef boost::signals2::signal<void(ClockEvent)> ClockSignal;
    ClockSignal    clockSignal;
public :
        ClockSignal* getClockSignal();
    
private:
	bool mShouldQuit;
	shared_ptr<thread> mThread;
    
    ParameterRefI bang;
    ParameterRefF bpm;
    void onBPMChange(ParameterEventF event);
    void onBANGChange(ParameterEventI event);
    static const uint16_t StepPerBeat = 840;
	static constexpr const float subStepInterval = 1.0f/StepPerBeat;
	static Math::LISTOF_FRACTION_LIST FRACTIONS;
	double interval;
    
	void run();
    
	Clock(float BPM = 60);
public :
    static ClockRef create(float BPM = 60){
        return ClockRef( new Clock(BPM) );
    }
    virtual ~Clock();
    
    void setBPM(float BPM);
    virtual ModuleRef display(int x, int y, int w, int h) override;
    virtual void hide() override;
};

//////////////////////////////////////

Math::LISTOF_FRACTION_LIST Clock::FRACTIONS = Math::getFractions(StepPerBeat);

using namespace ci;
using namespace std;
using namespace std::chrono;
using namespace boost::signals2;

typedef shared_ptr<class Clock> ClockRef;

Clock::Clock(float BPM):
    Module()
{
	bang = addSubModule<ParameterI>("bang", ParameterI::create(1, 0, 1));
    bpm = addSubModule<ParameterF>("bpm", ParameterF::create(BPM, 1, 300));
    
    clockSignal.connect([&](ClockEvent event) -> void{
        if(event.is(1, 1)){
            bang->setValue(1);
        }else if(event.is(1, 2)){
            bang->setValue(0);
        }
    });
    setBPM(BPM);
	mShouldQuit = false;
	mThread = shared_ptr<thread>(new thread(bind(&Clock::run, this)));
}

Clock::~Clock(){
	mShouldQuit = true;
    mThread->join();
    clockSignal.disconnect_all_slots();
}

void Clock::run(){
	typedef duration<double> DURATION;
	typedef high_resolution_clock CLCK;

	ThreadSetup threadSetup;
	double delay = 0;
	int counter = 0;
	auto t1 = CLCK::now();
	while(!mShouldQuit){
		auto t2 = CLCK::now();
		delay += duration_cast<DURATION>(t2 - t1).count();
		if(delay >= interval){
			delay -= interval;
            clockSignal({
				subStepInterval,
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
	bpm->setValue(BPM);
	interval = 60 / (double)(BPM * StepPerBeat);
}

signal<void(ClockEvent)>* Clock::getClockSignal(){
    return &clockSignal;
}

void Clock::onBPMChange(ParameterEventF event){
    setBPM(event.value);
    pannel->getSubView<UISliderF>("bpm")->setCursor(event.n_value);
}

void Clock::onBANGChange(ParameterEventI event){
    if(event.value == 1){
        pannel->getSubView<View>("bang")->setBgColor(Theme::bgActiveColor);
    }
    if(event.value == 0){
        pannel->getSubView<View>("bang")->setBgColor(Theme::bgDisactiveColor);
    }
}

ModuleRef Clock::display(int x, int y, int w, int h){
    Module::display(x, y, w, h);
    
    pannel->addSubView<View>("bang", View::create({10, 20}, {15, 15}))
        ->setBgColor(Theme::bgActiveColor);
    
    pannel->addSubView<UISliderF>("bpm", UISliderF::create({10, 40}, {size.x-20, 15}))
        ->setParameter(bpm);
    
    bpm->addEventListener(boost::bind(&Clock::onBPMChange, this, _1));
    bang->addEventListener(boost::bind(&Clock::onBANGChange, this, _1));
    
    
    return shared_from_this();
}

void Clock::hide(){
    Module::hide();
    bpm->removeEventListener(boost::bind(&Clock::onBPMChange, this, _1));
    bang->removeEventListener(boost::bind(&Clock::onBANGChange, this, _1));
    
}

#endif /* Clock_h */
