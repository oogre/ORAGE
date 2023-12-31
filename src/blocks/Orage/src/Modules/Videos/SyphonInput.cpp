//
//  SyphonInput.cpp
//  PianoProjete
//
//  Created by Vincent Evrard on 2017-05-29.
//
//

#include "SyphonInput.h"

using namespace ogre;
using namespace cinder;
using namespace cinder::gl;
using namespace ci::app;
using namespace std;
using namespace reza::ui;

namespace ogre {
    
    int SyphonInput::COUNT = 0;
    
    SyphonInput::SyphonInput( std::string name, JsonTree jsonData, vec2 origin, vec2 size, gl::Context * mMainWinCtx ) : ModuleVideo(name+" "+ tools.to_roman(SyphonInput::COUNT), origin, size, 0, 1, true){
        
        this->mMainWinCtx = mMainWinCtx;
        if(jsonData.hasChild("serverName")){
            serverName = jsonData.getChild("serverName").getValue<string>();
        }
    }
    
    void SyphonInput::setup(){
        ModuleVideo::setup();
        mFbo = gl::Fbo::create(FBO_WIDTH,
                               FBO_HEIGHT,
                               gl::Fbo::Format()
                               .attachment(GL_COLOR_ATTACHMENT0,
                                           gl::Texture2d::create(FBO_WIDTH,
                                                                 FBO_HEIGHT)));
        //glDel
        //GLuint depthTextureId = mFbo->getDepthTexture()->getId();
        
        outputs['A'] = mFbo->getTexture2d( GL_COLOR_ATTACHMENT0 );
        setupShader();
        setupUI();
        
        clientRef = new syphonClient();
        clientRef->setup();
        
        serverDir = new syphonServerDirectory();
        serverDir->setup();
        
        if(this->serverName != ""){
            std::vector<syphonServerDescription> servers = serverDir->getServerList();
            for(auto server = servers.begin() ; server != servers.end() ; server ++){
                cout<<server->serverName<<endl;
                if(serverName != server->serverName) continue;
                this->setClient(*server);
                break;
            }
        }
    }
    
    void SyphonInput::update(){
        if(mMainWinCtx != gl::Context::getCurrent()){
            return;
        }
        ModuleVideo::update();
        
        gl::pushMatrices();
        gl::ScopedViewport scpVp( ivec2( 0 ), mFbo->getSize() );
        gl::setMatrices( ModuleVideo::CAM );
        mFbo->bindFramebuffer();
        {
            gl::clear( ColorA(0, 0, 0, 0));
            gl::color(ColorA(1, 1, 1, 1));
            if(serverDir->isValidIndex(dirIdx))
                clientRef->draw(0, mFbo->getHeight(), mFbo->getWidth(), -mFbo->getHeight());
        }
        mFbo->unbindFramebuffer();
        
        gl::popMatrices();
    }
    
    void SyphonInput::setupUI(){
        
        ModuleVideo::setupUI();
        mUi->setColorBack(ColorAT<float>(vec4(.8f, .4f, .1f, .4f)));
        mUi->setColorFillHighlight(ColorAT<float>(vec4(.1f, .9f, 1.f, 1.f)));
        
        ButtonRef b = mUi->addButton("Next Client", false);
        
        b->setCallback(
                       [this](bool a) {
                           if(a){
                               this->nextClient();
                           }
                       });
        mUi->setMinified(false);
    }
    
    void SyphonInput::nextClient(){
        //press any key to move through all available Syphon servers
        dirIdx++;
        if(dirIdx > serverDir->size() - 1){
            dirIdx = 0;
        }
        
        setClient(dirIdx);
    }
    
    void SyphonInput::setClient(int _idx){
        if(serverDir->isValidIndex(_idx)){
            syphonServerDescription desc = serverDir->getDescription(_idx);
            setClient(desc);
        }
    }
    
    void SyphonInput::setClient(syphonServerDescription desc){
        try{
            serverName = desc.serverName;
            clientRef->set(desc);
            clientRef->bind();
        } catch (...) {}
    }
    
    int SyphonInput::getLastClient(){
        return serverDir->size()-1;
    }
    
    void SyphonInput::setupShader(){
        ModuleVideo::setupShader();
    }
}
