//
//  ParameterOsc.h
//  ORAGE
//
//  Created by Vincent Evrard on 8/11/21.
//

#ifndef ParameterNumber_h
#define ParameterNumber_h

#include "OrageInputNumber.h"
#include "ParameterBase.h"
#include "ISFAttr.h"

namespace reza {
    namespace ui {
        using namespace ISF;
        
        class ParameterNumber : public ParameterBase {
            typedef std::shared_ptr<ParameterNumber> ParameterNumberRef;
            ParameterNumber( ISF::ISFAttrRef & attr ) :
                ParameterBase( attr, PARAMETER_TYPE::NUMBER)
            {   
                labelRef = Label::create(attr->name(), attr->name(), FontSize::SMALL);
                
                TextInput::Format tFormat = TextInput::Format().numeric(true).fontSize(FontSize::MEDIUM).trigger(Trigger::CHANGE);
                inputRef = OrageInputNumber::create(attr->currentVal().getValString(), tFormat);
                inputRef->setCallback([attr](std::string value){
                    attr->currentVal().setValString(value);
                    attr->eventTrigger({ "change", attr });
                });
                
                auto action = [this, attr](int inc){
                    return [this, attr, inc](std::string value){
                        if(attr->isFloat()){
                            attr->currentVal().setDoubleVal(attr->currentVal().getDoubleVal() + (float)inc);
                        }
                        if(attr->isLong()){
                            attr->currentVal().setLongVal(attr->currentVal().getLongVal() + (long)inc);
                        }
                        attr->eventTrigger({ "change", attr });
                        inputRef->setValue(attr->currentVal().getValString());
                    };
                };
                
                inputRef->setIncCallback(action(1));
                
                inputRef->setDecCallback(action(-1));
                
                attr->addEventListener([this, attr](Evt evt){
                    if (evt.is("change") && evt.target != attr && inputRef->isEnabled()) {
                        inputRef->setValue(evt.target->currentVal().getValString());
                    }
                });
            }
        public :
            
            static ParameterNumberRef create( ISF::ISFAttrRef & attr )
            {
                return ParameterNumberRef( new ParameterNumber( attr ) );
            }
            
            virtual ~ParameterNumber(){
                cout<<"~ParameterNumber"<<endl;
                removeSubView(labelRef->getName());
                removeSubView(inputRef->getName());
                removeSubView(labelRef->getName());
                removeSubView(inputRef->getName());
                labelRef->clear();
                inputRef->clear();
            }
            
            virtual void setEnabled(bool value) override {
                ParameterBase::setEnabled(value);
                inputRef->setEnabled(value);
                labelRef->setEnabled(value);
            }
            
            LabelRef labelRef;
            OrageInputNumberRef inputRef;
            
        };//ParameterNumber
        typedef std::shared_ptr<ParameterNumber> ParameterNumberRef;
    }//ui {
}//reza {
#endif /* ParameterNumber_h */
