//
//  ModuleOscServer.h
//  ORAGE
//
//  Created by Vincent Evrard on 7/12/21.
//

#ifndef ModuleOscServer_h
#define ModuleOscServer_h
#include "Module.h"
#include "Osc.h"
#include "cinder/Log.h"
namespace ORAGE {
    namespace COMPONENTS {
        using namespace ci;
        using namespace ci::gl;
        using namespace ci::app;
        using namespace std;
        using namespace ISF;
        using Receiver = osc::ReceiverUdp;
        using Sender = osc::SenderUdp;
        using protocol = asio::ip::udp;
        
        class ModuleOscServer : public Module{
            typedef shared_ptr<ModuleOscServer> ModuleOscServerRef;
            std::string outHost = "127.0.0.1";
            
            ParameterNumberRef inPortUI;
            ParameterNumberRef outPortUI;
            TextInputRef outHostUI;
            ToggleRef    runUi;
            
            Sender    * mSender = nullptr;
            Receiver  * mReceiver = nullptr;
            
            ISFAttrRef attrOut;
            ISFAttrRef attrIn;
            ISFAttrRef portIn;
            ISFAttrRef portOut;
            
            
            
            void runHandler(bool value){
                inPortUI->setEnabled(!value);
                outPortUI->setEnabled(!value);
                outHostUI->setEnabled(!value);
                if(value){
                    std::string outHost = outHostUI->getValue();
                    
                    
                    mSender = new Sender(0, outHost, portOut->currentVal().getLongVal() );
                    mReceiver = new Receiver(portIn->currentVal().getLongVal());
                    try {
                        mSender->bind();
                        mReceiver->bind();
                    } catch ( const osc::Exception &ex ) {
                        CI_LOG_E( "Error binding: " << ex.what() << " val: " << ex.value() );
                        runUi->setValue(false);
                        inPortUI->setEnabled(value);
                        outPortUI->setEnabled(value);
                        outHostUI->setEnabled(value);
                        return;
                    }
                    
                    mReceiver->listen( []( asio::error_code error, protocol::endpoint endpoint ) -> bool {
                        if( error ) {
                            CI_LOG_E(
                                 "Error Listening: " <<
                                 error.message() <<
                                 " val: " <<
                                 error.value() <<
                                 " endpoint: " <<
                                 endpoint
                            );
                            return false;
                        }
                        return true;
                    });
                    
                    mReceiver->setListener( "*", [this]( const osc::Message &msg ){
                        attrOut->currentVal().setOscMessage(msg);
                        attrOut->eventTrigger({
                            "change", attrOut
                        });
                    });
                    attrIn->addEventListener(boost::bind(&ModuleOscServer::sendHandler, this, _1));
                }else{
                    cout<< "OSC close : " << endl;
                    attrIn->removeEventListener(boost::bind(&ModuleOscServer::sendHandler, this, _1));
                    mSender->close();
                    mReceiver->close();
                }
            }
            
            void sendHandler(Evt evt){
                if(evt.is("change")){
                    mSender->send(evt.target->currentVal().getOscMessage());
                }
            }
        protected:
            virtual void UIReady() override {
                Module::UIReady();
                inPortUI = UI->addNumber(portIn);
                UI->addSpacer();
                outHostUI = UI->addTextInput(outHost);
                outPortUI = UI->addNumber(portOut);
                UI->addSpacer();
                
                runUi = UI->addToggle("run", false, Toggle::Format().label(true).align(Alignment::CENTER));
                runUi->setCallback(boost::bind(&ModuleOscServer::runHandler, this, _1));
                
                UI->addOsc(attrOut);
                UI->addOsc(attrIn);
                
                UI->autoSizeToFitSubviews();
            }
            
        public :
            ModuleOscServer(string name, TYPES type) :
            Module(name)
            {
                moduleType = type;
                
                portIn = _attributes->addAttr(
                                              ISFAttr::create(
                                                              "portIn", "", "",
                                                              ISF::ISFAttr_IO::_IN,
                                                              ISFValType::ISFValType_Long,
                                                              ISFLongVal(0),
                                                              ISFLongVal(numeric_limits<long>::max()),
                                                              ISFLongVal(10000)
                                                              )
                                              );
                
                portOut = _attributes->addAttr(
                                               ISFAttr::create(
                                                               "portOut", "", "",
                                                               ISF::ISFAttr_IO::_OUT,
                                                               ISFValType::ISFValType_Long,
                                                               ISFLongVal(0),
                                                               ISFLongVal(numeric_limits<long>::max()),
                                                               ISFLongVal(10001)
                                                               )
                                               );
                
                attrOut = _attributes->addAttr(
                                               ISFAttr::create(
                                                               "out", "", "",
                                                               ISF::ISFAttr_IO::_OUT,
                                                               ISFValType::ISFValType_OscMessage,
                                                               ISFNullVal(),
                                                               ISFNullVal(),
                                                               ISFVal(ISFValType::ISFValType_OscMessage, osc::Message())
                                                               )
                                               );
                
                attrIn = _attributes->addAttr(
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
            virtual ~ModuleOscServer(){
                cout<<"~ModuleOscServer"<<endl;
                if(mSender != nullptr){
                    mSender->close();
                    delete mSender;
                }
                
                if(mReceiver != nullptr){
                    mReceiver->close();
                    delete mReceiver;
                }
            }
            static ModuleOscServerRef create(string name, TYPES type = TYPES::OSC){
                return std::make_shared<ModuleOscServer>(name, type);
            }
        };//ModuleOscServer
        typedef shared_ptr<ModuleOscServer> ModuleOscServerRef;
    }//COMPONENTS
}//ORAGE

#endif /* ModuleOscServer_h */
