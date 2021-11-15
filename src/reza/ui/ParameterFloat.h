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
        
            
            ParameterFloat( const ISF::ISFAttrRef & attr) :
                ParameterBase(attr->name())
            {
                type = PARAMETER_TYPE::FLOAT | (attr->IO() == ISF::ISFAttr_IO::_IN ? PLUG_TYPE::_IN : PLUG_TYPE::_OUT);
                sliderRef = OrageSliderT<double>::create(
                    attr->name(),
                    attr->currentVal().getDoubleValPtr(),
                    attr->minVal().getDoubleVal(),
                    attr->maxVal().getDoubleVal(),
                    OrageSliderT<double>::Format().value(true).precision(2).label(true).crossFader(true)
                );
                buttonRef = Button::create( attr->name()+"-Connector", false, Button::Format().label(false).circle(true));
                auto bgColor = getCableColor(true);
                bgColor.a = 1.0f;
                buttonRef->setColorOutline(getCableColor(true));
                buttonRef->setColorOutlineHighlight(ColorA::white());
                buttonRef->setCallback([&, attr](bool value) {
                    if(value){
                        EvtHandler::eventTrigger({
                            "plug", std::dynamic_pointer_cast<ParameterFloat>(shared_from_this())
                        });
                    }
                });
                limiterRef = RangeT<double>::create( attr->name()+"-Limiter", attr->minVal().getDoubleVal(), attr->maxVal().getDoubleVal(),  attr->minVal().getDoubleVal(), attr->maxVal().getDoubleVal(), Ranged::Format().label(false));
                limiterRef->setCallback([&](double a, double b) {
                    if(a != b){
                        sliderRef->setMinAndMax(std::min(a, b), std::max(a, b), true);
                        if(sliderRef->getValue() > std::max(a, b) || sliderRef->getValue() < std::min(a, b)){
                            sliderRef->setValue( lmap<double>( (double)sliderRef->getNormalizedValue(), 0.0, 1.0, sliderRef->getMin(), sliderRef->getMax()));
                        }
                    }
                });
            }
        public :
            static ParameterFloatRef create( const ISF::ISFAttrRef & attr ){
                return ParameterFloatRef( new ParameterFloat( attr ) );
            }
            virtual ~ParameterFloat(){
                
            }
            virtual void plugTo(std::shared_ptr<ParameterBase> other) override {
                ParameterFloatRef _other = std::dynamic_pointer_cast<ParameterFloat>(other);
                sliderRef->setSlave(_other->sliderRef);
                _other->sliderRef->setSlave(sliderRef);
            }
            virtual void unplugTo(std::shared_ptr<ParameterBase> other) override {
                ParameterFloatRef _other = std::dynamic_pointer_cast<ParameterFloat>(other);
                sliderRef->removeSlave(_other->sliderRef);
                _other->sliderRef->removeSlave(sliderRef);
            }
          
            OrageSliderdRef sliderRef;
            RangedRef limiterRef;
        };//ParameterFloat
        typedef std::shared_ptr<ParameterFloat> ParameterFloatRef;
    }//ui {
}//reza {

#endif /* ParameterFloat_h */
