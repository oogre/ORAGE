//
//  ParameterFloat.h
//  CinderProject
//
//  Created by Vincent Evrard on 7/10/21.
//

#ifndef ParameterFloat_h
#define ParameterFloat_h

#include "OrageSliderT.h"
#include "ParameterBase.h"
#include "ISFAttr.h"

namespace reza {
    namespace ui {
        class ParameterFloat : public ParameterBase {
            typedef std::shared_ptr<ParameterFloat> ParameterFloatRef;
        
            
            ParameterFloat( ISF::ISFAttrRef & attr) :
                ParameterBase(attr->name())
            {
                type = PARAMETER_TYPE::FLOAT | (attr->IO() == ISF::ISFAttr_IO::_IN ? PLUG_TYPE::_IN : PLUG_TYPE::_OUT);
                OrageSliderT<double>::Format sFormat = OrageSliderT<double>::Format().value(true).precision(2).label(true).crossFader(true);
                
                sliderRef = OrageSliderT<double>::create(attr->name(), attr->currentVal().getDoubleValPtr(), attr->minVal().getDoubleVal(), attr->maxVal().getDoubleVal(), sFormat);
                sliderRef->setCallback([&, attr](double val){
                    attr->update();
                });
                buttonRef = Button::create( attr->name()+"-Connector", false, Button::Format().label(false).circle(true));
                auto bgColor = getCableColor(true);
                bgColor.a = 1.0f;
                buttonRef->setColorOutline(getCableColor(true));
                buttonRef->setColorOutlineHighlight(ColorA::white());
                buttonRef->setCallback([&, attr](bool value) {
                    if(value){
                        EvtHandler::eventTrigger({
                            "plug", attr
                        });
                    }
                });
                attr->setPlug(buttonRef);
                limiterRef = RangeT<double>::create( attr->name()+"-Limiter", attr->minVal().getDoubleVal(), attr->maxVal().getDoubleVal(),  attr->minVal().getDoubleVal(), attr->maxVal().getDoubleVal(), Ranged::Format().label(false));
                limiterRef->setCallback([&, attr](double a, double b) {
                    if(a != b){
                        attr->setMin(std::min(a, b));
                        attr->setMax(std::max(a, b));
                    }
                });
            }
        public :
            static ParameterFloatRef create( ISF::ISFAttrRef & attr ){
                return ParameterFloatRef( new ParameterFloat( attr ) );
            }
            virtual void setVisible( bool visible ) override{
                ParameterBase::setVisible(visible);
                buttonRef->setVisible(visible);
                sliderRef->setVisible(visible);
                limiterRef->setVisible(visible);
            }
            
            virtual ~ParameterFloat(){
                
            }
            ButtonRef buttonRef;
            OrageSliderdRef sliderRef;
            RangedRef limiterRef;
        };//ParameterFloat
        typedef std::shared_ptr<ParameterFloat> ParameterFloatRef;
    }//ui {
}//reza {

#endif /* ParameterFloat_h */
