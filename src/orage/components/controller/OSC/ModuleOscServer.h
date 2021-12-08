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
            
            Sender    * mSender;
            Receiver  * mReceiver;
            
            ModuleOscServer(string name, TYPES type) :
                Module(name)
            {
                moduleType = type;
            }
            
        protected:
            virtual void UIReady() override
            {
                Module::UIReady();
                UI->addLabel("INPUT");
                auto inPortUI = UI->addTextInput(inPort, TextInput::Format().numeric());
                UI->addSpacer();
                UI->addLabel("OUTPUT");
                auto ipUI = UI->addTextInput(outHost);
                auto outPortUI = UI->addTextInput(outPort, TextInput::Format().numeric());
                UI->addSpacer();
                auto runUi = UI->addToggle("run", false, Toggle::Format().label(true).align(Alignment::CENTER));
                runUi->setCallback([this, inPortUI, ipUI, outPortUI, runUi](bool value){
                        if(value){
                            uint16_t inPort = atoi(inPortUI->getValue().c_str());
                            uint16_t outPort = atoi(outPortUI->getValue().c_str());
                            std::string outHost = ipUI->getValue();
                            cout<< "OSC open : " << endl;
                            cout<< "inPort : " << inPort << endl;
                            cout<< "outPort : " << outPort << endl;
                            cout<< "outHost : " << outHost << endl;
                            mSender = new Sender(0, outHost, outPort );
                            mReceiver = new Receiver(inPort);
                            try {
                                mSender->bind();
                            } catch ( const osc::Exception &ex ) {
                                CI_LOG_E( "Error binding: " << ex.what() << " val: " << ex.value() );
                                runUi->setValue(false);
                            }
                            try {
                                mReceiver->bind();
                            } catch( const osc::Exception &ex ) {
                                CI_LOG_E( "Error binding: " << ex.what() << " val: " << ex.value() );
                                runUi->setValue(false);
                            }
                            mReceiver->listen( [runUi]( asio::error_code error, protocol::endpoint endpoint ) -> bool {
                                if( error ) {
                                    CI_LOG_E( "Error Listening: " << error.message() << " val: " << error.value() << " endpoint: " << endpoint );
                                    runUi->setValue(false);
                                    return false;
                                }
                                return true;
                            });
                            
                            mReceiver->setListener( "*", [&]( const osc::Message &msg ){
                                cout<<msg.getAddress()<< " " << msg.getTypeTagString() << endl;
                                std::string tagType = msg.getTypeTagString();
                                for(int i = 0 ; i < tagType.length() ; i ++){
                                    switch(tagType.at(i)){
                                        case 'f' :
                                            cout<< msg.getArgFloat(i) << endl;
                                        break;
                                        default :
                                            cout<<tagType.at(i) << " not handled"<<endl;
                                        break;
                                    }
                                }
                            });
                        }else{
                            cout<< "OSC close : " << endl;
                            mSender->close();
                            mReceiver->close();
                        }
                    });
                
                ISFVal TIMEDELTAmin (ISFValType::ISFValType_Float, 0.0);
                ISFVal TIMEDELTAmax (ISFValType::ISFValType_Float, numeric_limits<double>::max());
                ISFVal TIMEDELTAval (ISFValType::ISFValType_Float, 0.0);
                ISFAttrRef attrOut = _attributes->addAttr(ISFAttr::create("out", "", "", ISF::ISFAttr_IO::_OUT, ISFValType::ISFValType_Clock, TIMEDELTAmin, TIMEDELTAmax, TIMEDELTAval));
                
                ISFAttrRef attrIn = _attributes->addAttr(ISFAttr::create("in", "", "", ISF::ISFAttr_IO::_IN, ISFValType::ISFValType_Clock, TIMEDELTAmin, TIMEDELTAmax, TIMEDELTAval));
                
                UI->addOsc(attrOut);
                UI->addOsc(attrIn);
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
