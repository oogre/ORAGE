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
        
            
            
        public :
            ParameterFloat( ISF::ISFAttrRef & attr) :
            ParameterBase(attr, PARAMETER_TYPE::FLOAT)
            {
                typedef OrageSliderT<double> Slider;
                Slider::Format sFormat = Slider::Format().value(true).precision(2).label(true).crossFader(true);
                sliderRef = Slider::create(attr->name(), attr->currentVal().getDoubleVal(), attr->minVal().getDoubleVal(), attr->maxVal().getDoubleVal(), sFormat);
                
                sliderRef->setCallback([this](double val){
                    getAttr()->currentVal().setDoubleVal(val);
                    getAttr()->eventTrigger({ "change", getAttr() });
                });
                
                attr->addEventListener([this](Evt evt){
                    ISF::ISFAttrRef attr = getAttr();
                    if (evt.is("change") && evt.target != attr && !!attr) {
                        
                        double tMin = evt.target->minVal().getDoubleVal();
                        double tMax = evt.target->maxVal().getDoubleVal();
                        double tVal = evt.target->currentVal().getDoubleVal();
                        
                        double cMin = attr->minVal().getDoubleVal();
                        double cMax = attr->maxVal().getDoubleVal();
                        
                        sliderRef->setValue(ci::lerp(cMin, cMax, InverseLerp(tMin, tMax, tVal)));
                    }
                });
                
                limiterRef = RangeT<double>::create( attr->name()+"-Limiter", attr->minVal().getDoubleVal(), attr->maxVal().getDoubleVal(),  attr->minVal().getDoubleVal(), attr->maxVal().getDoubleVal(), Ranged::Format().label(false));
                limiterRef->setCallback([this](double a, double b) {
                    if(a != b){
                        getAttr()->setMin(std::min(a, b));
                        getAttr()->setMax(std::max(a, b));
                    }
                });
            }
            static ParameterFloatRef create( ISF::ISFAttrRef & attr ){
                return std::make_shared<ParameterFloat>( attr );
            }
            virtual void setVisible( bool visible ) override{
                ParameterBase::setVisible(visible);
                sliderRef->setVisible(visible);
                limiterRef->setVisible(visible);
            }
            
            virtual ~ParameterFloat(){
                cout<<"~ParameterFloat " <<endl;
                removeSubView(sliderRef->getName());
                removeSubView(limiterRef->getName());
                removeSubView(sliderRef->getName());
                removeSubView(limiterRef->getName());
                sliderRef->clear();
                limiterRef->clear();
            }
            OrageSliderdRef sliderRef;
            RangedRef limiterRef;
        };//ParameterFloat
        typedef std::shared_ptr<ParameterFloat> ParameterFloatRef;
    }//ui {
}//reza {

#endif /* ParameterFloat_h */
