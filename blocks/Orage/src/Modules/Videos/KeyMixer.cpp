//
//  KeyMixer.cpp
//  orage
//
//  Created by Vincent Evrard on 2017-04-23.
//
//

#include "KeyMixer.h"


using namespace ogre;
using namespace cinder;
using namespace cinder::gl;
using namespace std;
using namespace reza::ui;

namespace ogre {
    
    int KeyMixer::COUNT = 0;
    
    KeyMixer::KeyMixer( std::string name, JsonTree jsonData, vec2 origin, vec2 size, gl::Context * mMainWinCtx ) : ModuleVideo(name+" "+ tools.to_roman(KeyMixer::COUNT), origin, size, 3, 1, true)
    {
        this->mMainWinCtx = mMainWinCtx;
    }
    
    void KeyMixer::setup(){
        ModuleVideo::setup();
        mFbo = gl::Fbo::create(FBO_WIDTH,
                               FBO_HEIGHT,
                               gl::Fbo::Format()
                               .attachment(GL_COLOR_ATTACHMENT0,
                                           gl::Texture2d::create(FBO_WIDTH,
                                                                 FBO_HEIGHT)));
        outputs['A'] = mFbo->getColorTexture();
        
        oldInputs['A'] = inputs['A'];
        oldInputs['B'] = inputs['B'];
        oldInputs['C'] = inputs['C'];
        
        setupShader();
        setupUI();
        
    }
    
    void KeyMixer::update(){
        
        if(mMainWinCtx != gl::Context::getCurrent()){
            return;
        }
        ModuleVideo::update();
        
        if(oldInputs['A'] != inputs['A']){
            inputA->setTexture(inputs['A']);
            oldInputs['A'] = inputs['A'];
        }
        
        if(oldInputs['B'] != inputs['B']){
            inputB->setTexture(inputs['B']);
            oldInputs['B'] = inputs['B'];
        }
        
        if(oldInputs['C'] != inputs['C']){
            inputC->setTexture(inputs['C']);
            oldInputs['C'] = inputs['C'];
        }
        
        gl::pushMatrices();
        gl::ScopedViewport scpVp( ivec2( 0 ), mFbo->getSize() );
        gl::setMatrices( ModuleVideo::CAM );
        
        mFbo->bindFramebuffer();
        {
            gl::clear( ColorA(0, 0, 0, 0));
            gl::ScopedGlslProg glslProg( mShader );
            
            if(inputs['A']){
                inputs['A']->bind(0);
                mShader->uniform( "tex0", 0 );  // texunit 0
                
                //mShader->uniform( "alpha0", a );  // texunit
            }else{
                //mShader->uniform( "alpha0", 0.f );  // texunit
            }
            if(inputs['B']){
                inputs['B']->bind(1);
                mShader->uniform( "tex1", 1 );  // texunit
                
                //mShader->uniform( "alpha1", b );  // texunit
                
            }else{
                //mShader->uniform( "alpha1", 0.f );  // texunit
            }
            if(inputs['C']){
                inputs['C']->bind(2);
                mShader->uniform( "tex2", 2 );  // texunit
                //mShader->uniform( "alpha2", b );  // texunit
                
            }else{
                //mShader->uniform( "alpha2", 0.f );  // texunit
            }

            gl::color(Color::white());
            gl::drawSolidRect(Area( 0, 0, mFbo->getWidth(), mFbo->getHeight() ));
            
            
            if(inputs['A']){
                inputs['A']->unbind(0);
            }
            if(inputs['B']){
                inputs['B']->unbind(1);
            }
            if(inputs['C']){
                inputs['C']->unbind(1);
            }
            
        }
        mFbo->unbindFramebuffer();
        gl::popMatrices();
        
    }
    
    void KeyMixer::setupUI(){
        ModuleVideo::setupUI();
        mUi->setColorBack(ColorAT<float>(vec4(.8f, .2f, .3f, .4f)));
        //mUi->setColorFill(ColorAT<float>(vec4(.8f, .9f, 1.f, .6f)));
        mUi->setColorFillHighlight(ColorAT<float>(vec4(.1f, .9f, 1.f, 1.f)));
        
        {
            inputA = TextureView::create("inputA", inputs['A']);
            
            float w = mUi->getWidth()*0.33 - mUi->mPadding.mLeft - mUi->mPadding.mRight;
            float h = h = ( w * ModuleVideo::RATIO );
            inputA->setSize( vec2( w, h ) );
            mUi->addSubViewDown(inputA);
        }
        {
            inputB = TextureView::create("inputB", inputs['B']);
            float w =  mUi->getWidth()*0.33 - mUi->mPadding.mLeft - mUi->mPadding.mRight;
            float h = h = ( w * ModuleVideo::RATIO );
            
            inputB->setSize( vec2( w, h ) );
            mUi->addSubViewRight(inputB);
        }
        {
            inputC = TextureView::create("inputC", inputs['B']);
            float w =  mUi->getWidth()*0.33 - mUi->mPadding.mLeft - mUi->mPadding.mRight;
            float h = h = ( w * ModuleVideo::RATIO );
            
            inputC->setSize( vec2( w, h ) );
            mUi->addSubViewRight(inputC);
        }
        
        
        //tools.addSlider(mUi, "Crossfade", &(crossfade), -1.f, 1.f, 260);
        
        
        mUi->setMinified(true);
    }
    
    void KeyMixer::setupShader(){
        ModuleVideo::setupShader();
        mShader = gl::GlslProg::create(
                                       loadAsset( "shaders/keymixer.vert"),
                                       loadAsset( "shaders/keymixer.frag")
                                       );
    }
    
}
