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


namespace reza {
    namespace ui {
        typedef std::shared_ptr<class ParameterFloat> ParameterFloatRef;
        
        class ParameterFloat
        : public ParameterBase {
            typedef std::shared_ptr<class ParameterFloat> ParameterFloatRef;
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
                friend class ParameterFloat;
            };
        private : 
            ParameterFloat( std::string name, double * value, double min, double max, Format format = Format()) : ParameterBase(name), mFormat(format)
            {
                if(!mFormat.mInput){
                    type = PARAMETER_TYPE::FLOAT | PLUG_TYPE::OUT;
                }else{
                    type = PARAMETER_TYPE::FLOAT | PLUG_TYPE::IN;
                }
                sliderRef = OrageSliderT<double>::create( name, value, min, max, OrageSliderT<double>::Format().value(true).precision(2).label(true).crossFader(true) );
                buttonRef = Button::create( name+"-Connector", false, Button::Format().label(false).circle(true));
                auto bgColor = getCableColor(true);
                bgColor.a = 1.0f;
                buttonRef->setColorOutline(getCableColor(true));
                buttonRef->setColorOutlineHighlight(ColorA::white());
                buttonRef->setCallback([&](bool a) {
                    if(a){
                        EvtHandler::eventTrigger({
                            "plug", std::dynamic_pointer_cast<ParameterFloat>(shared_from_this())
                        });
                    }
                });
                
                limiterRef = RangeT<double>::create( name+"-Limiter", min, max,  min, max, Ranged::Format().label(false));
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
            
            static ParameterFloatRef create( const std::string name, double * val, double min = 0.0, double max = 255, Format format = Format() )
            {
                return ParameterFloatRef( new ParameterFloat( name, val, min, max, format ) );
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
            virtual void setVisible( bool visible ) override{
                ParameterBase::setVisible(visible);
                buttonRef->setVisible(visible);
                limiterRef->setVisible(visible);
                sliderRef->setVisible(visible);
            }
            OrageSliderdRef sliderRef;
            RangedRef limiterRef;
            Format mFormat;
        };//ParameterFloat
        typedef std::shared_ptr<class ParameterFloat> ParameterFloatRef;
    }//ui {
}//reza {

#endif /* ParameterFloat_h */
