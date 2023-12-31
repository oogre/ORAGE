//
//  Spliter.cpp
//  orage
//
//  Created by Vincent Evrard on 2017-04-20.
//
//

#include "Spliter.h"



using namespace ogre;
using namespace cinder;
using namespace cinder::gl;
using namespace std;
using namespace reza::ui;

namespace ogre {
    
    int Spliter::COUNT = 0;
    
    Spliter::Spliter( std::string name, JsonTree jsonData, vec2 origin, vec2 size, gl::Context * mMainWinCtx ) : ModuleVideo(name+" "+ tools.to_roman(Spliter::COUNT), origin, size, 2, 2, true){
        this->mMainWinCtx = mMainWinCtx;
    }
    
    void Spliter::setup(){
        ModuleVideo::setup();
        mFbo = gl::Fbo::create(FBO_WIDTH,
                               FBO_HEIGHT,
                               gl::Fbo::Format()
                               .attachment(GL_COLOR_ATTACHMENT0,
                                           gl::Texture2d::create(FBO_WIDTH,
                                                                 FBO_HEIGHT))
                               .attachment(GL_COLOR_ATTACHMENT1,
                                           gl::Texture2d::create(FBO_WIDTH,
                                                                 FBO_HEIGHT)));
                               
        outputs['A'] = mFbo->getTexture2d( GL_COLOR_ATTACHMENT0 );
        outputs['B'] = mFbo->getTexture2d( GL_COLOR_ATTACHMENT1 );
        
        setupShader();
        setupUI();
    }
    
    void Spliter::update(){
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
            gl::ScopedGlslProg glslProg( mShader );
            
            if(inputs['A'] && inputs['B']){
                inputs['A']->bind(0);
                mShader->uniform( "tex0", 0 );  // texunit 0
                inputs['B']->bind(1);
                mShader->uniform( "tex1", 1 );  // texunit
            }
            gl::color(Color::white());
            gl::drawSolidRect(Area( 0, 0, mFbo->getWidth(), mFbo->getHeight() ));
            if(inputs['A'] && inputs['B']){
                inputs['A']->unbind(0);
                inputs['B']->unbind(1);
            }

        }
        mFbo->unbindFramebuffer();
        
        gl::popMatrices();
    }
    
    void Spliter::setupUI(){
        ModuleVideo::setupUI();
        mUi->setColorBack(ColorAT<float>(vec4(.8f, .4f, .1f, .4f)));
        //mUi->setColorFill(ColorAT<float>(vec4(.8f, .9f, 1.f, .6f)));
        mUi->setColorFillHighlight(ColorAT<float>(vec4(.1f, .9f, 1.f, 1.f)));
        
        mUi->setMinified(false);
    }
    
    void Spliter::setupShader(){
        
        ModuleVideo::setupShader();
        mShader = gl::GlslProg::create(
                                       loadAsset( "shaders/spliter.vert"),
                                       loadAsset( "shaders/spliter.frag")
                                       );
    }
    
}
