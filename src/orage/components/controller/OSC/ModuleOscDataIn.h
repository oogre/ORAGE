//
//  ModuleOscData.h
//  ORAGE
//
//  Created by Vincent Evrard on 7/12/21.
//

#ifndef ModuleOscDataIn_h
#define ModuleOscDataIn_h
#include "Module.h"
namespace ORAGE {
    namespace COMPONENTS {
        using namespace ci;
        using namespace ci::gl;
        using namespace ci::app;
        using namespace std;
        using namespace ISF;
        
        class ModuleOscDataIn : public Module {
            typedef shared_ptr<ModuleOscDataIn> ModuleOscDataInRef;
            
            std::string address = "*";
            ci::osc::Message old;
            ISFAttrWrapperRef createLater;
            std::vector<std::string> destroyLater;
            TextInputRef addressUI;
            TextInputRef typeTagUI;
            ISFAttrRef inAttr;
            
            ModuleOscDataIn(string name, TYPES type) :
                Module(name)
            {
                moduleType = type;
                createLater = ISFAttrWrapper::create();
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

            }
            void readyHandler(bool value){
                typeTagUI->setEnabled(!value);
                addressUI->setEnabled(!value);
                std::string typeTag = typeTagUI->getValue();
                std::string address = addressUI->getValue();
                if(value){
                    for(int i = 0 ; i < typeTag.length() ; i++){
                        switch(typeTag.at(i)){
                            case 'f' :
                            case 'd' :
                                createLater->addAttr(
                                     ISFAttr::create(
                                         address+"."+to_string(i), "", "",
                                         ISF::ISFAttr_IO::_OUT,
                                         ISFValType::ISFValType_Float,
                                         ISFVal(ISFValType::ISFValType_Float, 0.0),
                                         ISFVal(ISFValType::ISFValType_Float, 1.0),
                                         ISFVal(ISFValType::ISFValType_Float, 0.0)
                                     )
                                 );
                            break;
                            default :
                                cout<<typeTag.at(i) << " not handled"<<endl;
                            break;
                        }
                    }
                    inAttr->addEventListener(boost::bind(&ModuleOscDataIn::receiveHandler, this, _1));
                }else{
                    for(int i = 0 ; i < typeTag.length() ; i++){
                        destroyLater.push_back(address+"."+to_string(i));
                        UI->getParameter(address+"."+to_string(i))->eventTrigger({
                            "rmplug", _attributes->get(address+"."+to_string(i))
                        });
                    }
                    inAttr->removeEventListener(boost::bind(&ModuleOscDataIn::receiveHandler, this, _1));
                }
            }
            
            void receiveHandler(Evt evt){
                if (evt.is("change")) {
                    ci::osc::Message msg = evt.target->currentVal().getOscMessage();
                    std::string address = addressUI->getValue();
                    std::string typeTag = typeTagUI->getValue();
                    if("*" == address || address == msg.getAddress()){
                        for(int i = 0 ; i < typeTag.length() ; i ++){
                            ISFAttrRef attr = _attributes->get(address+"."+to_string(i));
                            ParameterFloatRef attrUI = dynamic_pointer_cast<ParameterFloat>(UI->getParameter(address+"."+to_string(i)));
                            switch(typeTag.at(i)){
                                case 'f' :
                                    attr->currentVal().setDoubleVal(msg.getArgFloat(i));
                                    attrUI->sliderRef->setValue(msg.getArgFloat(i));
                                    attr->eventTrigger({
                                        "change", attr
                                    });
                                    break;
                                case 'd' :
                                    attr->currentVal().setDoubleVal(msg.getArgDouble(i));
                                    attrUI->sliderRef->setValue(msg.getArgDouble(i));
                                    attr->eventTrigger({
                                        "change", attr
                                    });
                                    break;
                                default :
                                    cout<<typeTag.at(i) << " not handled"<<endl;
                                    break;
                            }
                        }
                    }
                }
            }
            void updateUI(){
                if(!createLater->isEmpty()){
                    for(auto attr : createLater->every()){
                        _attributes->addAttr(attr);
                        ISFAttrRef a = _attributes->get(attr->name());
                        UI->addParameter(a)->addEventListener(parameterPlugHandler);
                        UI->autoSizeToFitSubviews();
                    }
                    createLater->clear();
                }
                if(destroyLater.size()>0){
                    for(auto name : destroyLater){
                        UI->rmParameter(name);
                        _attributes->rmAttr(name);
                        UI->setNeedsDisplay();
                        UI->autoSizeToFitSubviews();
                    }
                    destroyLater.clear();
                }
            }
        protected:
            virtual void UIReady() override {
                Module::UIReady();
                UI->addLabel("address");
                addressUI = UI->addTextInput(address);
                UI->addLabel("typeTag");
                typeTagUI = UI->addTextInput("f");
                
                UI->addToggle("ready", false, Toggle::Format().label(true).align(Alignment::CENTER))
                    ->setCallback(boost::bind(&ModuleOscDataIn::readyHandler, this, _1));

                UI->addOsc(inAttr);
                UI->autoSizeToFitSubviews();
            }
        public :
            virtual ~ModuleOscDataIn(){
            }
            
            virtual void update() override {
                Module::update();
                updateUI();
            }
            
            static ModuleOscDataInRef create(string name, TYPES type = TYPES::OSC){
                return ModuleOscDataInRef(new ModuleOscDataIn(name, type));
            }
        };//ModuleOscDataIn
        typedef shared_ptr<ModuleOscDataIn> ModuleOscDataInRef;
    }//COMPONENTS
}//ORAGE

#endif /* ModuleOscDataIn_h */
