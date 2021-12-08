//
//  ModuleOscData.h
//  ORAGE
//
//  Created by Vincent Evrard on 7/12/21.
//

#ifndef ModuleOscData_h
#define ModuleOscData_h
#include "Module.h"
namespace ORAGE {
    namespace COMPONENTS {
        using namespace ci;
        using namespace ci::gl;
        using namespace ci::app;
        using namespace std;
        using namespace ISF;
        
        class ModuleOscData : public Module{
            typedef shared_ptr<ModuleOscData> ModuleOscDataRef;
            
            std::string address = "*";
            ci::osc::Message old;
            ISFAttrWrapperRef createLater;
            std::vector<std::string> destroyLater;
            TextInputRef addressUI;
            TextInputRef typeTagUI;
            
            ModuleOscData(string name, TYPES type) :
                Module(name)
            {
                moduleType = type;
                createLater = ISFAttrWrapper::create();
            }
            
        protected:
            virtual void UIReady() override
            {
                Module::UIReady();
                UI->addLabel("address");
                addressUI = UI->addTextInput(address);
                UI->addLabel("typeTag");
                typeTagUI = UI->addTextInput("f");
                
                auto readyUi = UI->addToggle("ready", false, Toggle::Format().label(true).align(Alignment::CENTER));
                readyUi->setCallback([this](bool value){
                    typeTagUI->setEnabled(!value);
                    addressUI->setEnabled(!value);
                    std::string typeTag = typeTagUI->getValue();
                    std::string address = addressUI->getValue();
                    if(value){
                        for(int i = 0 ; i < typeTag.length() ; i++){
                            switch(typeTag.at(i)){
                                case 'f' :
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
                        _attributes->get("in")->addEventListener(boost::bind(&ModuleOscData::receiveHandler, this, _1));
                    }else{
                        for(int i = 0 ; i < typeTag.length() ; i++){
                            destroyLater.push_back(address+"."+to_string(i));
                            UI->getParameter(address+"."+to_string(i))->eventTrigger({
                                "unplug", _attributes->get(address+"."+to_string(i))
                            });
                        }
                        _attributes->get("in")->removeEventListener(boost::bind(&ModuleOscData::receiveHandler, this, _1));
                    }
                });
                    
                typeTagUI->setCallback([this](std::string value){
                    
                });
                
                _attributes->addAttr(
                    ISFAttr::create(
                      "out", "", "",
                      ISF::ISFAttr_IO::_OUT,
                      ISFValType::ISFValType_OscMessage,
                      ISFNullVal(),
                      ISFNullVal(),
                      ISFVal(ISFValType::ISFValType_OscMessage, osc::Message())
                    )
                );
                
                _attributes->addAttr(
                     ISFAttr::create(
                         "in", "", "",
                         ISF::ISFAttr_IO::_IN,
                         ISFValType::ISFValType_OscMessage,
                         ISFNullVal(),
                         ISFNullVal(),
                         ISFVal(ISFValType::ISFValType_OscMessage, osc::Message())
                     )
                 );
                
                ISFAttrRef outAttr = _attributes->get("out");
                ISFAttrRef inAttr = _attributes->get("in");
                UI->addOsc(outAttr);
                UI->addOsc(inAttr);
                UI->autoSizeToFitSubviews();
            }
            
            void receiveHandler(Evt evt){
                if (evt.is("change")) {
                    ci::osc::Message current = evt.target->currentVal().getOscMessage();
                    std::string address = current.getAddress();
                    std::string typeTag = current.getTypeTagString();
                    if(address == addressUI->getValue() && typeTag == typeTagUI->getValue()){
                        for(int i = 0 ; i < typeTag.length() ; i ++){
                            switch(typeTag.at(i)){
                                case 'f' :
                                    _attributes->get(address+"."+to_string(i))->setCurrent(current.getArgFloat(i));
                                    break;
                                default :
                                    cout<<typeTag.at(i) << " not handled"<<endl;
                                    break;
                            }
                        }
                    }
                }
            }
        public :
            virtual ~ModuleOscData(){
            }
            
            virtual void update() override {
                for(auto attr : createLater->every()){
                    _attributes->addAttr(attr);
                    ISFAttrRef a = _attributes->get(attr->name());
                    UI->addParameter(a)->addEventListener(parameterPlugHandler);
                    UI->autoSizeToFitSubviews();
                }
                createLater->clear();
                for(auto name : destroyLater){
                    UI->rmParameter(name);
                    _attributes->rmAttr(name);
                    UI->setNeedsDisplay();
                    UI->autoSizeToFitSubviews();
                }
                destroyLater.clear();
            }
            
            static ModuleOscDataRef create(string name, TYPES type = TYPES::OSC){
                return ModuleOscDataRef(new ModuleOscData(name, type));
            }
        };//ModuleOscData
        typedef shared_ptr<ModuleOscData> ModuleOscDataRef;
    }//COMPONENTS
}//ORAGE

#endif /* ModuleOscData_h */
