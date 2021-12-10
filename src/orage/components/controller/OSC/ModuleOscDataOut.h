//
//  ModuleOscData.h
//  ORAGE
//
//  Created by Vincent Evrard on 7/12/21.
//

#ifndef ModuleOscDataOut_h
#define ModuleOscDataOut_h
#include "Module.h"
namespace ORAGE {
    namespace COMPONENTS {
        using namespace ci;
        using namespace ci::gl;
        using namespace ci::app;
        using namespace std;
        using namespace ISF;
        
        class ModuleOscDataOut : public Module{
            typedef shared_ptr<ModuleOscDataOut> ModuleOscDataOutRef;
            
            std::string address = "*";
            ci::osc::Message old;
            ISFAttrWrapperRef createLater;
            std::vector<std::string> destroyLater;
            TextInputRef addressUI;
            TextInputRef typeTagUI;
            ISFAttrRef outAttr;
            
            ModuleOscDataOut(string name, TYPES type) :
                Module(name)
            {
                moduleType = type;
                createLater = ISFAttrWrapper::create();
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
                                         ISF::ISFAttr_IO::_IN,
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
                }else{
                    for(int i = 0 ; i < typeTag.length() ; i++){
                        UI->getParameter(address+"."+to_string(i))->eventTrigger({
                            "rmplug", _attributes->get(address+"."+to_string(i))
                        });
                        destroyLater.push_back(address+"."+to_string(i));
                    }
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
                    ->setCallback(boost::bind(&ModuleOscDataOut::readyHandler, this, _1));
                
                UI->addOsc(outAttr);
                UI->autoSizeToFitSubviews();
            }
            
            void SenderHandler(Evt evt){
                if (evt.is("change")) {
                    std::string address = addressUI->getValue();
                    std::string typeTag = typeTagUI->getValue();
                    osc::Message msg;
                    msg.setAddress(address);
                    for(int i = 0 ; i < typeTag.length() ; i++){
                        std::string name = address+"."+to_string(i);
                        switch(typeTag.at(i)){
                            case 'f' :
                                msg.append((float)_attributes->get(name)->currentVal().getDoubleVal());
                                break;
                            case 'd' :
                                msg.append((double)_attributes->get(name)->currentVal().getDoubleVal());
                                break;
                            default :
                                cout<<typeTag.at(i) << " not handled"<<endl;
                                break;
                        }
                    }
                    outAttr->currentVal().setOscMessage(msg);
                    outAttr->eventTrigger({ "change", outAttr });
                }
            }
            void updateUI(){
                if(!createLater->isEmpty()){
                    for(auto attr : createLater->every()){
                        _attributes->addAttr(attr);
                        attr->addEventListener(boost::bind(&ModuleOscDataOut::SenderHandler, this, _1));
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
        public :
            virtual ~ModuleOscDataOut(){
                
            }
            
            virtual void update() override {
                Module::update();
                updateUI();
            }
            
            static ModuleOscDataOutRef create(string name, TYPES type = TYPES::OSC){
                return ModuleOscDataOutRef(new ModuleOscDataOut(name, type));
            }
        };//ModuleOscData
        typedef shared_ptr<ModuleOscDataOut> ModuleOscDataOutRef;
    }//COMPONENTS
}//ORAGE

#endif /* ModuleOscDataOut_h */
