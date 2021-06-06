#ifndef Clock_h
#define Clock_h

#include "../../lib/chrono_io.h"
#include "../UI/IView.h"
#include "../UI/Slider.h"
#include "Parameter.h"
#include "Math.h"
#include "Module.h"
#include <boost/signals2.hpp>

struct ClockEvent{
	float time;
	float progress;
	Math::FRACTION_LIST list;
	bool is(int num, int den);
};

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
    
	static const uint16_t StepPerBeat = 840;
	static constexpr const float subStepInterval = 1.0f/StepPerBeat;
	static Math::LISTOF_FRACTION_LIST FRACTIONS;
	double interval;
    
	void update();
    
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
    Module("Clock")
{
	bang = dynamic_pointer_cast<ParameterI>(addSubModule("bang", ParameterI::create(1, 0, 1)));
    bpm = dynamic_pointer_cast<ParameterF>(addSubModule("bpm", ParameterF::create(BPM, 1, 300)));
    
    clockSignal.connect([&](ClockEvent event) -> void{
        if(event.is(1, 1)){
            bang->setValue(1);
        }else if(event.is(1, 2)){
            bang->setValue(0);
        }
    });
    setBPM(BPM);
	mShouldQuit = false;
	mThread = shared_ptr<thread>(new thread(bind(&Clock::update, this)));
}

Clock::~Clock(){
	mShouldQuit = true;
    mThread->join();
    clockSignal.disconnect_all_slots();
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
            clockSignal({
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
	bpm->setValue(BPM);
	interval = 60 / (double)(BPM * StepPerBeat);
}

signal<void(ClockEvent)>* Clock::getClockSignal(){
    return &clockSignal;
}

ModuleRef Clock::display(int x, int y, int w, int h){
    Module::display(x, y, w, h);
	view = IView::create(name, origin, size);
    
    IViewRef iview = dynamic_pointer_cast<IView>(view);
    ViewRef bangUI = view->addSubView<View>("bang", View::create(name, {10, 10}, {10, 10}));
    
    IViewRef bpmUI3 = view->addSubView<IView>("abc", IView::create(name, {10, 30}, {100, 15}));
    UISliderRef bpmUI = view->addSubView<UISlider>("bpm", UISlider::create(name, {10, 30}, {100, 15}));
    
    
    iview->bgColor = Theme::bgDisactiveColor;
    bangUI->bgColor = Theme::bgActiveColor;
    bpmUI->setCursor(bpm->getValue(true));
    
    iview->getSignal("enter")->connect([&](CustomEvent event) -> void{
        view->bgColor = Theme::bgActiveColor;
    });
    iview->getSignal("leave")->connect([&](CustomEvent event) -> void{
        view->bgColor = Theme::bgDisactiveColor;
    });
    bpmUI->getSignal("wheel")->connect([&](CustomEvent event) -> void{
        bpm->setValue(bpm->getValue() - event.mouseEvent.getWheelIncrement());
    });
    
    bpm->getChangeSignal()->connect([&](ParameterEvent<float> event) -> void{
        setBPM(event.value);
        view->getSubView<UISlider>("bpm")->setCursor(event.n_value);
    });
    bang->getChangeSignal()->connect([&](ParameterEvent<int> event) -> void{
        if(event.value == 1){
            view->getSubView<View>("bang")->bgColor = Theme::bgActiveColor;
        }
        if(event.value == 0){
            view->getSubView<View>("bang")->bgColor = Theme::bgDisactiveColor;
        }
    });
    return shared_from_this();
}

void Clock::hide(){
    Module::hide();
    bpm->getChangeSignal()->disconnect_all_slots();
    bang->getChangeSignal()->disconnect_all_slots();
}

//////////////////////////////////////

bool ClockEvent::is(int num, int den){
	return find(list.begin(), list.end(), make_pair(num, den)) != list.end();
}

#endif /* Clock_h */
