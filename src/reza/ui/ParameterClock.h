//
//  ParameterClock.h
//  ORAGE
//
//  Created by Vincent Evrard on 8/11/21.
//

#ifndef ParameterClock_h
#define ParameterClock_h

#include "ParameterBase.h"
#include "ISFAttr.h"

namespace reza {
    namespace ui {
        using namespace ISF;
        
        class ParameterClock : public ParameterBase {
            typedef std::shared_ptr<ParameterClock> ParameterClockRef;
            ParameterClock( const ISF::ISFAttrRef & attr ) :
                ParameterBase(attr->name())
            {
                ParameterBase::clockAttr = attr;
                ParameterBase::clockAttrIn = &(ParameterBase::clockAttr);
                
                type = PARAMETER_TYPE::CLOCK | (attr->IO() == ISF::ISFAttr_IO::_IN ? PLUG_TYPE::_IN : PLUG_TYPE::_OUT);
                
                buttonRef = Button::create( attr->name(), false, Button::Format().label(false).circle(true));
                auto bgColor = getCableColor(true);
                bgColor.a = 1.0f;
                buttonRef->setColorOutline(getCableColor(true));
                buttonRef->setColorOutlineHighlight(ColorA::white());
                buttonRef->setCallback([&](bool a) {
                    if(a){
                        EvtHandler::eventTrigger({
                            "plug", std::dynamic_pointer_cast<ParameterClock>(shared_from_this())
                        });
                    }
                });
            }
        public :
            
            static ParameterClockRef create( const ISF::ISFAttrRef & attr )
            {
                return ParameterClockRef( new ParameterClock( attr ) );
            }
            virtual ~ParameterClock(){
                
            }
            virtual void plugTo(std::shared_ptr<ParameterBase> other) override {
                ParameterClockRef _other = std::dynamic_pointer_cast<ParameterClock>(other);
                ParameterBase::clockAttrIn = &_other->clockAttr;
            }
            virtual void unplugTo(std::shared_ptr<ParameterBase> other) override {
                ParameterClockRef _other = std::dynamic_pointer_cast<ParameterClock>(other);
                ParameterBase::clockAttrIn = &(ParameterBase::clockAttr);
            }
            virtual void setVisible( bool visible ) override {
                ParameterBase::setVisible(visible);
                buttonRef->setVisible(visible);
            }
        };//ParameterClock
        typedef std::shared_ptr<ParameterClock> ParameterClockRef;
    }//ui {
}//reza {
#endif /* ParameterClock_h */
