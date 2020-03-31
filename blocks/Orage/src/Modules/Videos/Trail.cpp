//
//  Trail.cpp
//  ORAGE2
//
//  Created by Vincent Evrard on 2017-05-03.
//
//

#include "Trail.h"

using namespace ogre;
using namespace cinder;
using namespace cinder::gl;
using namespace std;
using namespace reza::ui;

namespace ogre {
    
    int Trail::COUNT = 0;
    
    Trail::Trail( std::string name, vec2 origin, vec2 size, gl::Context * mMainWinCtx ) : ModuleVideo(name+" "+ tools.to_roman(Trail::COUNT), origin, size, 2, 1, true){
        this->mMainWinCtx = mMainWinCtx;
    }
    
    void Trail::setup(){
        ModuleVideo::setup();
        mFbo = gl::Fbo::create(FBO_WIDTH,
                               FBO_HEIGHT,
                               gl::Fbo::Format()
                               .attachment(GL_COLOR_ATTACHMENT0,
                                           gl::Texture2d::create(FBO_WIDTH,
                                                                 FBO_HEIGHT)));
        
       
        mFbo2 = gl::Fbo::create(FBO_WIDTH,
                               FBO_HEIGHT,
                               gl::Fbo::Format()
                               .attachment(GL_COLOR_ATTACHMENT0,
                                           gl::Texture2d::create(FBO_WIDTH,
                                                                 FBO_HEIGHT)));
        
       
        outputs['A'] = mFbo->getColorTexture();
        setupShader();
        setupUI();
        
        // allocate our UBO
        dataUbo = gl::Ubo::create( sizeof( data ), &data, GL_DYNAMIC_DRAW );
        // and bind it to buffer base 0; this is analogous to binding it to texture unit 0
        dataUbo->bindBufferBase( id );
        // and finally tell the shaders that their uniform buffer 'FormulaParams' can be found at buffer base 0
        mShader->uniformBlock("trail", id );
        
        
    }
    
    void Trail::update(){
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
            dataUbo->bufferSubData( 0, sizeof( data ), &data );
            gl::ScopedGlslProg glslProg( mShader );

            mFbo2->getTexture2d(GL_COLOR_ATTACHMENT0)->bind(0);
            mShader->uniform( "tex0", 0 );  // texunit
            
            if(inputs['A']){
                inputs['A']->bind(1);
                mShader->uniform( "tex1", 1 );  // texunit 0
            }
            if(inputs['B']){
                inputs['B']->bind(2);
                mShader->uniform( "tex2", 2 );  // texunit
            }

            gl::color(Color::white());
            gl::drawSolidRect(Area( 0, 0, mFbo->getWidth(), mFbo->getHeight() ));
            mFbo2->getTexture2d(GL_COLOR_ATTACHMENT0)->unbind(0);
            if(inputs['A']){
                inputs['A']->unbind(1);
            }
            if(inputs['B']){
                inputs['B']->unbind(2);
            }
        }
        mFbo->unbindFramebuffer();
        
        mFbo2->bindFramebuffer();
        {
            gl::enableAlphaBlendingPremult();
            gl::color( 1, 1, 1, 1);
            gl::draw(mFbo->getColorTexture(), Area(0, 0, mFbo2->getWidth(), mFbo2->getHeight()));
        }
        mFbo2->unbindFramebuffer();
        
        gl::popMatrices();
    }
    
    void Trail::setupUI(){
        ModuleVideo::setupUI();
        mUi->setColorBack(ColorAT<float>(vec4(.1f, .7f, .3f, .4f)));
        //mUi->setColorFill(ColorAT<float>(vec4(.8f, .9f, 1.f, .6f)));
        mUi->setColorFillHighlight(ColorAT<float>(vec4(.3f, .9f, 1.f, 1.f)));
       
        tools.addSlider(mUi, "amount", &(data.amount), 0, 1.0f);
        tools.addSlider(mUi, "black", &(data.black), 0, 1.0f);
        tools.addSlider(mUi, "white", &(data.white), 0, 1.0f);
        
        mUi->setMinified(true);
    }
    
    void Trail::setupShader(){
        
        ModuleVideo::setupShader();
        mShader = gl::GlslProg::create(
                                       loadAsset( "shaders/trail.vert"),
                                       loadAsset( "shaders/trail.frag")
                                       );
    }
}