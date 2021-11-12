//
//  Syphon_Spout.h
//  ORAGE
//
//  Created by Vincent Evrard on 12/11/21.
//

#ifndef Syphon_Spout_h
#define Syphon_Spout_h

#include "cinder/app/App.h"
#include "ParameterTexture.h"
#if defined(VVGL_SDK_MAC)
#include "cinderSyphon.h"
#endif

#if defined(VVGL_SDK_WIN)
#include "SpoutLibrary.h"
#endif

namespace ORAGE {
    namespace COMMON {
        using namespace ci;
        using namespace std;
        using namespace ci::gl;
        
        class SyphonSpoutClient {
            typedef shared_ptr<SyphonSpoutClient> SyphonSpoutClientRef;
        #if defined(VVGL_SDK_MAC)
            syphonClient * client;
            syphonServerDirectory * serverDir;
            int dirIdx;
        #endif
        #if defined(VVGL_SDK_WIN)
            SPOUTLIBRARY * client;
            Texture2dRef texIn;
        #endif
            SyphonSpoutClient(){
            #if defined(VVGL_SDK_MAC)
                client = new syphonClient();
                client->setup();
                serverDir = new syphonServerDirectory();
                serverDir->setup();
                dirIdx = 0;
            #endif
            #if defined(VVGL_SDK_WIN)
                client = GetSpout();
                texIn = Texture2d::create(800, 600);
            #endif
            }
        public :
            virtual ~SyphonSpoutClient(){
            #if defined(VVGL_SDK_MAC)
                delete client;
                delete serverDir;
            #endif
            #if defined(VVGL_SDK_WIN)
                client->ReleaseReceiver();
                client->Release();
            #endif
            }
            static SyphonSpoutClientRef create(){
                return SyphonSpoutClientRef(new SyphonSpoutClient());
            }
            void nextClient(){
             #if defined(VVGL_SDK_MAC)
                dirIdx++;
                if(dirIdx >= serverDir->size()){
                    dirIdx = 0;
                }
                if(serverDir->isValidIndex(dirIdx)){
                    syphonServerDescription desc = serverDir->getDescription(dirIdx);
                    client->set(desc);
                    client->bind();
                }
            #endif
            #if defined(VVGL_SDK_WIN)
                client->SelectSender();
            #endif
            }
            Texture2dRef draw(ci::vec2 origine = ci::vec2(0), ci::vec2 size = ci::vec2(0)){
            #if defined(VVGL_SDK_MAC)
                if(serverDir->isValidIndex(dirIdx))
                    client->draw(origine, size);
            #endif
            #if defined(VVGL_SDK_WIN)
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
            reza::ui::ParameterTextureRef tex;
            bool isActive = false;
        #if defined(VVGL_SDK_MAC)
            syphonServer * server;
        #endif
        #if defined(VVGL_SDK_WIN)
            SPOUTLIBRARY * server;
        #endif
            SyphonSpoutServer(string name, reza::ui::ParameterTextureRef tex) :
                name(name),
                tex(tex)
            {
            
            }
        public :
            virtual ~SyphonSpoutServer(){
                disable();
            }
            static SyphonSpoutServerRef create(string name, reza::ui::ParameterTextureRef tex){
                return SyphonSpoutServerRef(new SyphonSpoutServer(name, tex));
            }
            
            void draw(){
                if(isActive){
                #if defined(VVGL_SDK_MAC)
                    server->publishTexture(tex->textureRef);
                #endif
                #if defined(VVGL_SDK_WIN)
                    server->SendTexture(tex->textureRef->getId(), tex->textureRef->getTarget(), tex->textureRef->getWidth(), tex->textureRef->getHeight(), false);
                #endif
                }
            }
            void enable(){
                if(!isActive){
                #if defined(VVGL_SDK_MAC)
                    server = new syphonServer();
                    server->setName(name);
                #endif
                #if defined(VVGL_SDK_WIN)
                    server = GetSpout();
                    server->SetSenderName(&name[0]);
                #endif
                    isActive = true;
                }
            }
            void disable(){
                if(isActive){
                 #if defined(VVGL_SDK_MAC)
                    delete server;
                #endif
                #if defined(VVGL_SDK_WIN)
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
