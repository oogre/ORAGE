//
//  EventTemplate.h
//  CinderProject
//
//  Created by Vincent Evrard on 6/10/21.
//

#ifndef EventTemplate_h
#define EventTemplate_h
#include "OrageMath.h"
#include <boost/signals2.hpp>
// https://scicomp.ethz.ch/public/manual/Boost/1.55.0/signals2.pdf
#include "ModuleTypes.h"
namespace ORAGE {
    namespace COMMON {
        using namespace std;
        using namespace boost::signals2;
        
        struct BaseEvent {
            public :
            string type;
            time_t timeStamp;
            BaseEvent(){
            }
            BaseEvent(string type) :
            type(type),
            timeStamp(time(0))
            {
            }
            bool is(string eventName){
                return eventName == type;
            }
        };//struct BaseEvent
        
        template<typename T>
        struct MenuEvent : public BaseEvent{
            public :
            T target;
            ci::vec2 origin;
            COMPONENTS::TYPES moduleType;
            MenuEvent(string type, T target, COMPONENTS::TYPES moduleType, ci::vec2 origin = ci::vec2(0)) :
            BaseEvent(type),
            target(target),
            moduleType(moduleType),
            origin(origin)
            {
            }
        };//struct MenuEvent<T>
        
        template<typename T>
        struct Event : public BaseEvent{
            typedef std::shared_ptr<T> Target;
            typedef std::vector<Target> Blacklist;
        public :
            Target target;
            Blacklist _blackList;
            Event(string type, Target target, Blacklist _blacklist = std::vector<shared_ptr<T>>()) :
            BaseEvent(type),
            target(target),
            _blackList(_blackList)
            {
            }
            void blacklist(Target elem){
                _blackList.push_back(elem);
            }
            bool isValid(Target other){
                return find(_blackList.begin(), _blackList.end(), other) == _blackList.end();
            }
        };//struct Event<T>
        
        template<typename T>
        struct MouseEvent : public COMMON::Event<T>{
            ci::app::MouseEvent mouseEvent;
            ci::ivec2 oldMousePos;
            MouseEvent(string type, shared_ptr<T> target) :
            COMMON::Event<T>(type, target)
            {
            }
            MouseEvent(string type, ci::app::MouseEvent mouseEvent, shared_ptr<T> target, ci::ivec2 oldMousePos) :
            COMMON::Event<T>(type, target),
            mouseEvent(mouseEvent),
            oldMousePos(oldMousePos)
            {
            };
        };//struct MouseEvent<T>
        
        struct ClockEvent : public BaseEvent{
            float timeUnit;
            float progress;
            OrageMath::FRACTION_LIST list;
            bool is(int num, int den){
                return find(list.begin(), list.end(), make_pair(num, den)) != list.end();
            }
            ClockEvent(string type, float timeUnit, float progress, OrageMath::FRACTION_LIST list):
            BaseEvent(type),
            timeUnit(timeUnit),
            progress(progress),
            list(list)
            {
            }
        };//struct ClockEvent
        
        template<typename E>
        class EventTemplate {
            typedef boost::signals2::signal<void(E)> EventSignal;
            typedef typename EventSignal::slot_type EventSignalSlot;
            EventSignal signal;
        protected :
            EventTemplate(){
            }
        public :
            virtual ~EventTemplate(){
                signal.disconnect_all_slots();
            }
            virtual  boost::signals2::connection addEventListener(EventSignalSlot slot) {
                return signal.connect(slot);
            }
            template<typename Callable>
            bool removeEventListener(Callable slot) {
                signal.disconnect(slot);
                return true;
            }
            
            virtual bool eventTrigger(E event) {
                signal(event);
                return true;
            }
        };//class EventTemplate
    }//namespace COMMON
}//namespace ORAGE

#endif /* EventTemplate_h */
