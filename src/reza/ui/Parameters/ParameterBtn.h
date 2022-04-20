//
//  ParameterToggle.h
//  ORAGE
//
//  Created by Vincent Evrard on 8/11/21.
//

#ifndef ParameterBtn_h
#define ParameterBtn_h

#include "ParameterBase.h"
#include "ISFAttr.h"
#include "UI.h"

namespace reza {
    namespace ui {
        using namespace ISF;
        
        class ParameterBtn : public ParameterBase {
            typedef std::shared_ptr<ParameterBtn> ParameterBtnRef;
            
        public :
            ParameterBtn( ISF::ISFAttrRef & attr ) :
                ParameterBase( attr, PARAMETER_TYPE::BOOLEAN )
            {
                bangRef = Button::create(attr->name(), attr->currentVal().getBoolVal(), Button::Format().label(true).align(Alignment::CENTER));
                bangRef->setCallback([this](bool val){
                    getAttr()->currentVal().setBoolVal(val);
                    getAttr()->eventTrigger({ "change", getAttr() });
                });
                attr->addEventListener([this](Evt evt){
                    ISF::ISFAttrRef attr = getAttr();
                    if (evt.is("change") && evt.target != attr && !!attr) {
                        bangRef->setValue(evt.target->currentVal().getBoolVal());
                    }
                });
            }
            
            static ParameterBtnRef create( ISF::ISFAttrRef & attr )
            {
                return std::make_shared<ParameterBtn>( attr );
            }
            
            virtual ~ParameterBtn(){
                cout<<"~ParameterBtn"<<endl;
            }
            
            virtual void setVisible( bool visible ) override{
                ParameterBase::setVisible(visible);
                bangRef->setVisible(visible);
            }
            
            ButtonRef bangRef;
            
        };//ParameterBtn
        typedef std::shared_ptr<ParameterBtn> ParameterBtnRef;
    }//ui {
}//reza {
#endif /* ParameterBtn_h */
