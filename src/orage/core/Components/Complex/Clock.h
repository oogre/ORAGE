#ifndef Clock_h
#define Clock_h

#include "../../../../lib/chrono_io.h"
#include "../../../common/Math.h"


namespace ORAGE {
    namespace CORE {
        using namespace ci;
        using namespace std;
        using namespace std::chrono;
        
        
        class Clock : public Module, public COMMON::EventTemplate<Clock, COMMON::ClockEvent> {
            typedef shared_ptr<class Clock> ClockRef;
            typedef shared_ptr<class thread> ThreadRef;
            typedef COMMON::EventTemplate<Clock, COMMON::ClockEvent> ClockEvt;
            typedef ORAGE::COMMON::Math Math;
            double interval;
            bool mShouldQuit;
            
            ThreadRef mThread;
            static const uint16_t StepPerBeat = 360;
            static const uint16_t defaultBPM = 60;
            static Math::LISTOF_FRACTION_LIST FRACTIONS;
            static constexpr const float subStepInterval = 1.0f/StepPerBeat;
            
            void run(){
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
                        ClockEvt::eventTrigger({
                            "tic",
                            subStepInterval,
                            (++counter) * subStepInterval,
                            Clock::FRACTIONS[counter-1]
                        });
                        if(counter >= StepPerBeat){
                            counter = 0;
                        }
                    }
                    t1 = t2;
                    this_thread::sleep_for(milliseconds(1));
                }
            }//void run()
            
            double bpmToInterval(float BPM){
                return 60 / (double)(BPM * StepPerBeat) ;
            }//double bpmToInterval(float BPM)
            
            Clock(string name, string type = "Clock"):
                Module(name, type),
                COMMON::EventTemplate<Clock, COMMON::ClockEvent>(),
                interval(bpmToInterval(defaultBPM))
            {
                addModule(NumberI::create("bang"));
                addModule(NumberI::create("bpm"))
                    ->addEventListener("change", [&](COMMON::Event<Module> event) -> void {
                        float bpm = event.target->as<NumberI>()->getValue();
                        interval = bpmToInterval(bpm);
                    });
                
                ClockEvt::addEventListener("tic", [&](COMMON::ClockEvent event) -> void {
                    if(event.is(1, 1)){
                        getModule("bang")->setValue(1);
                    }else if(event.is(1, 2)){
                        getModule("bang")->setValue(0);
                    }
                });
                
                mShouldQuit = false;
                mThread = ThreadRef(new thread(bind(&Clock::run, this)));
            }//Clock(string name, int BPM)
        public :
            static ClockRef create(string name){
                return ClockRef( new Clock(name) );
            }
            
            virtual ~Clock(){
                mShouldQuit = true;
                mThread->join();
                //clockSignal.disconnect_all_slots();
            }
            
            virtual void setConf(json conf) override {
                Module::setConf(conf);
                //getModule("bpm")->as<NumberI>()->setValue(conf.at("bpm"));
            }
        };// class Clock
        typedef shared_ptr<class Clock> ClockRef;
        ORAGE::COMMON::Math::LISTOF_FRACTION_LIST ORAGE::CORE::Clock::FRACTIONS = ORAGE::COMMON::Math::getFractions(StepPerBeat);
    }//namespace CORE
}//namespace ORAGE
#endif /* Clock_h */
