//
//  ModuleOscData.h
//  ORAGE
//
//  Created by Vincent Evrard on 7/12/21.
//

#ifndef ModuleOscAddress_h
#define ModuleOscAddress_h
#include "Module.h"
namespace ORAGE {
    namespace COMPONENTS {
        using namespace ci;
        using namespace ci::gl;
        using namespace ci::app;
        using namespace std;
        using namespace ISF;
        
        class ModuleOscAddress : public Module {
            typedef shared_ptr<ModuleOscAddress> ModuleOscAddressRef;
            
            std::string address = "*";
            TextInputRef addressUI;
            ISFAttrRef inAttr;
            ISFAttrRef outAttr;
            
            ModuleOscAddress(string name, TYPES type) :
                Module(name)
            {
                moduleType = type;
                outAttr = _attributes->addAttr(
                    ISFAttr::create(
                        "out", "", "",
                        ISF::ISFAttr_IO::_OUT,
                        ISFValType::ISFValType_OscMessage,
                        ISFNullVal(),
                        ISFNullVal(),
                        ISFVal(ISFValType::ISFValType_OscMessage, osc::Message())
                    )
                );
                inAttr = _attributes->addAttr(
                    ISFAttr::create(
                        "in", "", "",
                        ISF::ISFAttr_IO::_IN,
                        ISFValType::ISFValType_OscMessage,
                        ISFNullVal(),
                        ISFNullVal(),
                        ISFVal(ISFValType::ISFValType_OscMessage, osc::Message())
                    )
                );
                inAttr->addEventListener(boost::bind(&ModuleOscAddress::sendHandler, this, _1));
            }
            
            void sendHandler(Evt evt){
                if (evt.is("change")) {
                    ci::osc::Message msg = evt.target->currentVal().getOscMessage();
                    std::string address = addressUI->getValue();
                    msg.setAddress(address);
                    outAttr->currentVal().setOscMessage(msg);
                    outAttr->eventTrigger({
                        "change",
                        outAttr
                    });
                    
                }
            }
        protected:
            virtual void UIReady() override {
                Module::UIReady();
                UI->addLabel("address");
                addressUI = UI->addTextInput(address);
                UI->addOsc(inAttr);
                UI->addOsc(outAttr);
                UI->autoSizeToFitSubviews();
            }
        public :
            virtual ~ModuleOscAddress(){
            }
            
            virtual void update() override {
                Module::update();
            }
            
            static ModuleOscAddressRef create(string name, TYPES type = TYPES::OSC){
                return ModuleOscAddressRef(new ModuleOscAddress(name, type));
            }
        };//ModuleOscAddress
        typedef shared_ptr<ModuleOscAddress> ModuleOscAddressRef;
    }//COMPONENTS
}//ORAGE

#endif /* ModuleOscAddress_h */
