//
//  ParameterClock.h
//  ORAGE
//
//  Created by Vincent Evrard on 8/11/21.
//

#ifndef ParameterClock_h
#define ParameterClock_h

#include "ParameterBase.h"


namespace reza {
    namespace ui {
        using namespace ISF;
        
        class ParameterClock : public ParameterBase {
            typedef std::shared_ptr<ParameterClock> ParameterClockRef;
        public:
            struct Format {
            public:
                Format(){ input( true ); }
                Format( const Format &copy )
                {
                    mInput = copy.mInput;
                }
                Format &input( bool input = true )
                {
                    mInput = input;
                    return *this;
                }
                bool mInput;
            protected:
                friend ParameterClock;
            };
            private :
            ParameterClock( std::string name, ISFAttrRef clockAttr, Format format = Format()) :
                ParameterBase(name),
                mFormat(format)
            {
                ParameterBase::clockAttr = clockAttr;
                ParameterBase::clockAttrIn = &(ParameterBase::clockAttr);
                
                if(!mFormat.mInput){
                    type = PARAMETER_TYPE::CLOCK | PLUG_TYPE::OUT;
                }else{
                    type = PARAMETER_TYPE::CLOCK | PLUG_TYPE::IN;
                }
                
                buttonRef = Button::create( name+"-Clock", false, Button::Format().label(false).circle(true));
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
            
            static ParameterClockRef create( const std::string name, ISFAttrRef clockAttr, Format format = Format() )
            {
                return ParameterClockRef( new ParameterClock( name, clockAttr, format ) );
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
            virtual void setVisible( bool visible ) override{
                ParameterBase::setVisible(visible);
                buttonRef->setVisible(visible);
            }
            Format mFormat;
        };//ParameterClock
        typedef std::shared_ptr<ParameterClock> ParameterClockRef;
    }//ui {
}//reza {


#endif /* ParameterClock_h */
