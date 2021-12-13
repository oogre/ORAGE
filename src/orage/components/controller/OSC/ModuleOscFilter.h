//
//  ModuleOscData.h
//  ORAGE
//
//  Created by Vincent Evrard on 7/12/21.
//

#ifndef ModuleOscFilter_h
#define ModuleOscFilter_h
#include "Module.h"
namespace ORAGE {
    namespace COMPONENTS {
        using namespace ci;
        using namespace ci::gl;
        using namespace ci::app;
        using namespace std;
        using namespace ISF;
        
        class ModuleOscFilter : public Module {
            typedef shared_ptr<ModuleOscFilter> ModuleOscFilterRef;
            
            std::string address = "*";
            TextInputRef addressUI;
            ISFAttrRef inAttr;
            ISFAttrRef outAttr;
            
            
            
            void receiveHandler(Evt evt){
                if (evt.is("change")) {
                    ci::osc::Message msg = evt.target->currentVal().getOscMessage();
                    std::string address = addressUI->getValue();
                    if("*" == address || address == msg.getAddress()){
                        outAttr->currentVal().setOscMessage(msg);
                        outAttr->eventTrigger({
                            "change",
                            outAttr
                        });
                    }
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
            ModuleOscFilter(string name, TYPES type) :
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
                inAttr->addEventListener(boost::bind(&ModuleOscFilter::receiveHandler, this, _1));
            }
            virtual ~ModuleOscFilter(){
                cout<<"~ModuleOscFilter"<<endl;
            }
            
            virtual void update() override {
                Module::update();
            }
            
            static ModuleOscFilterRef create(string name, TYPES type = TYPES::OSC){
                return std::make_shared<ModuleOscFilter>(name, type);
            }
        };//ModuleOscFilter
        typedef shared_ptr<ModuleOscFilter> ModuleOscFilterRef;
    }//COMPONENTS
}//ORAGE

#endif /* ModuleOscFilter_h */
