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
#endif

namespace ORAGE {
    namespace COMMON {
        using namespace std;
        
        class SyphonSpoutClient {
            typedef shared_ptr<SyphonSpoutClient> SyphonSpoutClientRef;
            int dirIdx;
            
        #if defined(VVGL_SDK_MAC)
            syphonClient * client;
            syphonServerDirectory * serverDir;
        #endif
        #if defined(VVGL_SDK_WIN)
        #endif
            SyphonSpoutClient(){
                dirIdx = 0 ;
            #if defined(VVGL_SDK_MAC)
                client = new syphonClient();
                client->setup();
                serverDir = new syphonServerDirectory();
                serverDir->setup();
            #endif
            #if defined(VVGL_SDK_WIN)
            #endif
            }
        public :
            virtual ~SyphonSpoutClient(){
            #if defined(VVGL_SDK_MAC)
                delete client;
                delete serverDir;
            #endif
            #if defined(VVGL_SDK_WIN)
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
                #endif
            }
            void draw(ci::vec2 origine, ci::vec2 size){
                if(serverDir->isValidIndex(dirIdx))
                    client->draw(origine, size);
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
                #endif
                    isActive = false;
                }
            }
        };//SyphonSpoutServer
        typedef shared_ptr<SyphonSpoutServer> SyphonSpoutServerRef;
    }//COMMON
}//ORAGE
#endif /* Syphon_Spout_h */
