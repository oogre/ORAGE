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
            std::string outPort = "10001";
            std::string inPort = "10000";
            
            TextInputRef inPortUI;
            TextInputRef outPortUI;
            TextInputRef outHostUI;
            ToggleRef    runUi;
            
            Sender    * mSender;
            Receiver  * mReceiver;
            
            ISFAttrRef attrOut;
            ISFAttrRef attrIn;
            
            ModuleOscServer(string name, TYPES type) :
                Module(name)
            {
                moduleType = type;
                
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
            
            void runHandler(bool value){
                inPortUI->setEnabled(!value);
                outPortUI->setEnabled(!value);
                outHostUI->setEnabled(!value);
                if(value){
                    uint16_t inPort = atoi(inPortUI->getValue().c_str());
                    uint16_t outPort = atoi(outPortUI->getValue().c_str());
                    std::string outHost = outHostUI->getValue();
                    
                    cout<< "OSC open : " << endl;
                    cout<< "inPort : " << inPort << endl;
                    cout<< "outPort : " << outPort << endl;
                    cout<< "outHost : " << outHost << endl;
                    
                    mSender = new Sender(0, outHost, outPort );
                    mReceiver = new Receiver(inPort);
                    try {
                        mSender->bind();
                        mReceiver->bind();
                    } catch ( const osc::Exception &ex ) {
                        CI_LOG_E( "Error binding: " << ex.what() << " val: " << ex.value() );
                        runUi->setValue(false);
                        return;
                    }
                    
                    mReceiver->listen( [this]( asio::error_code error, protocol::endpoint endpoint ) -> bool {
                        if( error ) {
                            CI_LOG_E(
                                 "Error Listening: " <<
                                 error.message() <<
                                 " val: " <<
                                 error.value() <<
                                 " endpoint: " <<
                                 endpoint
                            );
                            runUi->setValue(false);
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
                UI->addLabel("INPUT");
                inPortUI = UI->addTextInput(inPort, TextInput::Format().numeric());
                UI->addSpacer();
                UI->addLabel("OUTPUT");
                outHostUI = UI->addTextInput(outHost);
                outPortUI = UI->addTextInput(outPort, TextInput::Format().numeric());
                UI->addSpacer();
                runUi = UI->addToggle("run", false, Toggle::Format().label(true).align(Alignment::CENTER));
                runUi->setCallback(boost::bind(&ModuleOscServer::runHandler, this, _1));
                
                UI->addOsc(attrOut);
                UI->addOsc(attrIn);
                UI->autoSizeToFitSubviews();
            }
            
        public :
            virtual ~ModuleOscServer(){
                mSender->close();
                mReceiver->close();
                delete mReceiver;
                delete mSender;
            }
            static ModuleOscServerRef create(string name, TYPES type = TYPES::OSC){
                return ModuleOscServerRef(new ModuleOscServer(name, type));
            }
        };//ModuleOscServer
        typedef shared_ptr<ModuleOscServer> ModuleOscServerRef;
    }//COMPONENTS
}//ORAGE

#endif /* ModuleOscServer_h */
