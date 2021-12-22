//
//  ParameterToggle.h
//  ORAGE
//
//  Created by Vincent Evrard on 8/11/21.
//

#ifndef ParameterToggle_h
#define ParameterToggle_h

#include "ParameterBase.h"
#include "ISFAttr.h"
#include "UI.h"

namespace reza {
    namespace ui {
        using namespace ISF;
        
        class ParameterToggle : public ParameterBase {
            typedef std::shared_ptr<ParameterToggle> ParameterToggleRef;
            
        public :
            ParameterToggle( ISF::ISFAttrRef & attr ) :
                ParameterBase( attr, PARAMETER_TYPE::BOOLEAN )
            {
                toggleRef = Toggle::create(attr->name(), attr->currentVal().getBoolVal(), Button::Format().label(true).align(Alignment::CENTER));
                toggleRef->setCallback([this](bool val){
                    getAttr()->currentVal().setBoolVal(val);
                    getAttr()->eventTrigger({ "change", getAttr() });
                });
                attr->addEventListener([this](Evt evt){
                    ISF::ISFAttrRef attr = getAttr();
                    if (evt.is("change") && evt.target != attr && !!attr) {
                        toggleRef->setValue(evt.target->currentVal().getBoolVal());
                    }
                });
            }
            
            static ParameterToggleRef create( ISF::ISFAttrRef & attr )
            {
                return std::make_shared<ParameterToggle>( attr );
            }
            
            virtual ~ParameterToggle(){
                cout<<"~ParameterToggle"<<endl;
            }
            
            virtual void setVisible( bool visible ) override{
                ParameterBase::setVisible(visible);
                toggleRef->setVisible(visible);
            }
            
            ToggleRef toggleRef;
            
        };//ParameterToggle
        typedef std::shared_ptr<ParameterToggle> ParameterToggleRef;
    }//ui {
}//reza {
#endif /* ParameterToggle_h */
