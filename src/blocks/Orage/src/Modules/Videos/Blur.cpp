//
//  Blur.cpp
//  PianoProjete
//
//  Created by Vincent Evrard on 2017-06-10.
//
//

#include "Blur.h"


using namespace ogre;
using namespace cinder;
using namespace cinder::gl;
using namespace std;
using namespace reza::ui;

namespace ogre {
    
    int Blur::COUNT = 0;
    
    Blur::Blur( std::string name, JsonTree jsonData, vec2 origin, vec2 size, gl::Context * mMainWinCtx) : ModuleVideo(name+" "+ tools.to_roman(Blur::COUNT), origin, size, 2, 1, true){
        if(jsonData.getNumChildren()!=0){
            data = DATA(jsonData);
        }
        this->mMainWinCtx = mMainWinCtx;
    }
    
    void Blur::setup(){
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
        
        outputs['A'] = mFbo2->getColorTexture();
        
        setupShader();
        setupUI();
        
        // allocate our UBO
        dataUbo = gl::Ubo::create( sizeof( sData ), &sData, GL_DYNAMIC_DRAW );
        // and bind it to buffer base 0; this is analogous to binding it to texture unit 0
        dataUbo->bindBufferBase( id );
        // and finally tell the shaders that their uniform buffer 'FormulaParams' can be found at buffer base 0
        mShader->uniformBlock("blur", id );
    }
    
    void Blur::update(){
        if(mMainWinCtx != gl::Context::getCurrent()){
            return;
        }
        ModuleVideo::update();
        
        sData.amountX = data.amountX.value;
        sData.amountY = data.amountY.value;
        
        gl::pushMatrices();
        gl::ScopedViewport scpVp( ivec2( 0 ), mFbo->getSize() );
        gl::setMatrices( ModuleVideo::CAM );
        
        mFbo->bindFramebuffer();
        {
            gl::clear( ColorA(0, 0, 0, 0));
            dataUbo->bufferSubData( 0, sizeof( sData ), &sData );
            gl::ScopedGlslProg glslProg( mShader );
            if(inputs['A']){
                inputs['A']->bind(0);
                mShader->uniform( "tex0", 0 );  // texunit 0
            }
            
            
            if(inputs['B']){
                inputs['B']->bind(1);
                mShader->uniform( "tex1", 1 );  // texunit
                sData.blurActive = 1;
            }else{
                sData.blurActive = 0;
            }
            
            gl::color(Color::white());
            gl::drawSolidRect(Area( 0, 0, mFbo->getWidth(), mFbo->getHeight() ));
            if(inputs['A']){
                inputs['A']->unbind(0);
            }
            if(inputs['B']){
                inputs['B']->unbind(1);
            }
        }
        mFbo->unbindFramebuffer();
        
        mFbo2->bindFramebuffer();
        {
            gl::enableAlphaBlendingPremult();
            gl::color( ColorA(1.0f,1.0f,1.0f,1.0f));
            gl::draw(mFbo->getColorTexture(), Area(0, 0, mFbo2->getWidth(), mFbo2->getHeight()));
        }
        mFbo2->unbindFramebuffer();
        
        gl::popMatrices();
    }
    
    void Blur::setupUI(){
        
        ModuleVideo::setupUI();
        mUi->setColorBack(ColorAT<float>(vec4(.8f, .1f, .8f, .4f)));
        //mUi->setColorFill(ColorAT<float>(vec4(.8f, .9f, 1.f, .6f)));
        mUi->setColorFillHighlight(ColorAT<float>(vec4(.1f, .9f, 1.f, 1.f)));
        
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        

        tools.addSlider(mUi, "X Blur", this->id, &(data.amountX));
        tools.addSlider(mUi, "Y Blur", this->id, &(data.amountY));
        

        
        mUi->setMinified(true);
    }
    
    void Blur::setupShader(){
        ModuleVideo::setupShader();
        mShader = gl::GlslProg::create(
                                       loadAsset( "shaders/blur.vert"),
                                       loadAsset( "shaders/blur.frag")
                                       );
    }
}
