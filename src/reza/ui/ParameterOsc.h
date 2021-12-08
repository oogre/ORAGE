//
//  ParameterOsc.h
//  ORAGE
//
//  Created by Vincent Evrard on 8/11/21.
//

#ifndef ParameterOsc_h
#define ParameterOsc_h

#include "ParameterBase.h"
#include "ISFAttr.h"

namespace reza {
    namespace ui {
        using namespace ISF;
        
        class ParameterOsc : public ParameterBase {
            typedef std::shared_ptr<ParameterOsc> ParameterOscRef;
            ParameterOsc( ISF::ISFAttrRef & attr ) :
                ParameterBase( attr->name() )
            {
                type = PARAMETER_TYPE::OSC | (attr->IO() == ISF::ISFAttr_IO::_IN ? PLUG_TYPE::_IN : PLUG_TYPE::_OUT);
                buttonRef = Button::create( attr->name(), false, Button::Format().label(false).circle(true));
                auto bgColor = getCableColor(true);
                bgColor.a = 1.0f;
                buttonRef->setColorOutline(getCableColor(true));
                buttonRef->setColorOutlineHighlight(ColorA::white());
                buttonRef->setCallback([&, attr](bool a) {
                    if(a){
                        EvtHandler::eventTrigger({
                            "plug", attr
                        });
                    }
                });
                attr->setPlug(buttonRef);
            }
        public :
            
            static ParameterOscRef create( ISF::ISFAttrRef & attr )
            {
                return ParameterOscRef( new ParameterOsc( attr ) );
            }
            
            virtual ~ParameterOsc(){
                
            }
            
            virtual void setVisible( bool visible ) override {
                ParameterBase::setVisible(visible);
                buttonRef->setVisible(visible);
            }
            ButtonRef buttonRef;
        };//ParameterOsc
        typedef std::shared_ptr<ParameterOsc> ParameterOscRef;
    }//ui {
}//reza {
#endif /* ParameterClock_h */
