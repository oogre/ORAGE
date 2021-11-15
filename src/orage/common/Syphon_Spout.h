//
//  Syphon_Spout.h
//  ORAGE
//
//  Created by Vincent Evrard on 12/11/21.
//

#ifndef Syphon_Spout_h
#define Syphon_Spout_h

#include "cinder/app/App.h"
#include "ISFAttr.h"
#if defined(CINDER_MAC)
#include "cinderSyphon.h"
#elif defined(CINDER_MSW)
#include "SpoutLibrary.h"
#endif

namespace ORAGE {
    namespace COMMON {
        using namespace ci;
        using namespace std;
        using namespace ci::gl;
        
        class SyphonSpoutClient {
            typedef shared_ptr<SyphonSpoutClient> SyphonSpoutClientRef;
        #if defined(CINDER_MAC)
            syphonClient * client;
            syphonServerDirectory * serverDir;
            int dirIdx;
        #elif defined(CINDER_MSW)
            SPOUTLIBRARY * client;
            Texture2dRef texIn;
        #endif
            SyphonSpoutClient(){
            #if defined(CINDER_MAC)
                client = new syphonClient();
                client->setup();
                serverDir = new syphonServerDirectory();
                serverDir->setup();
                dirIdx = 0;
            #elif defined(CINDER_MSW)
                client = GetSpout();
                texIn = Texture2d::create(800, 600);
            #endif
            }
        public :
            virtual ~SyphonSpoutClient(){
            #if defined(CINDER_MAC)
                delete client;
                delete serverDir;
            #elif defined(CINDER_MSW)
                client->ReleaseReceiver();
                client->Release();
            #endif
            }
            static SyphonSpoutClientRef create(){
                return SyphonSpoutClientRef(new SyphonSpoutClient());
            }
            void nextClient(){
             #if defined(CINDER_MAC)
                dirIdx++;
                if(dirIdx >= serverDir->size()){
                    dirIdx = 0;
                }
                if(serverDir->isValidIndex(dirIdx)){
                    syphonServerDescription desc = serverDir->getDescription(dirIdx);
                    client->set(desc);
                    client->bind();
                }
            #elif defined(CINDER_MSW)
                client->SelectSender();
            #endif
            }
            Texture2dRef draw(ci::vec2 origine = ci::vec2(0), ci::vec2 size = ci::vec2(0)){
            #if defined(CINDER_MAC)
                if(serverDir->isValidIndex(dirIdx))
                    client->draw(origine, size);
            #elif defined(CINDER_MSW)
                if (client->IsUpdated()) {
                    texIn = Texture2d::create(client->GetSenderWidth(), client->GetSenderHeight());
                }
                if (client->ReceiveTexture(texIn->getId(), texIn->getTarget())) {
                    return texIn;
                }
            #endif
                return nullptr;
            }
        };
        typedef shared_ptr<SyphonSpoutClient> SyphonSpoutClientRef;
        
        class SyphonSpoutServer {
            typedef shared_ptr<SyphonSpoutServer> SyphonSpoutServerRef;
            string name;
            ISF::ISFAttrRef attr;
            bool isActive = false;
        #if defined(CINDER_MAC)
            syphonServer * server;
        #elif defined(CINDER_MSW)
            SPOUTLIBRARY * server;
        #endif
            SyphonSpoutServer(string name, ISF::ISFAttrRef attr) :
                name(name),
                attr(attr)
            {
            
            }
        public :
            virtual ~SyphonSpoutServer(){
                disable();
            }
            static SyphonSpoutServerRef create(string name, ISF::ISFAttrRef attr){
                return SyphonSpoutServerRef(new SyphonSpoutServer(name, attr));
            }
            
            void draw(){
                Texture2dRef currentTex = attr->currentVal().imageBuffer();;
                
                if(isActive){
                #if defined(CINDER_MAC)
                    server->publishTexture(currentTex);
                #elif defined(CINDER_MSW)
                    server->SendTexture(currentTex->getId(), currentTex->getTarget(), currentTex->getWidth(), currentTex->getHeight(), false);
                #endif
                }
            }
            void enable(){
                if(!isActive){
                #if defined(CINDER_MAC)
                    server = new syphonServer();
                    server->setName(name);
                #elif defined(CINDER_MSW)
                    server = GetSpout();
                    server->SetSenderName(&name[0]);
                #endif
                    isActive = true;
                }
            }
            void disable(){
                if(isActive){
                 #if defined(CINDER_MAC)
                    delete server;
                #elif defined(CINDER_MSW)
                    server->ReleaseSender();
                    server->Release();
                #endif

                    isActive = false;
                }
            }
        };//SyphonSpoutServer
        typedef shared_ptr<SyphonSpoutServer> SyphonSpoutServerRef;
    }//COMMON
}//ORAGE
#endif /* Syphon_Spout_h */
