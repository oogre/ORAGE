//
//  Delay.cpp
//  PianoProjete
//
//  Created by Vincent Evrard on 2017-06-01.
//
//

#include "Delay.h"


using namespace ogre;
using namespace cinder;
using namespace cinder::gl;
using namespace std;
using namespace reza::ui;

namespace ogre {
    
    int Delay::COUNT = 0;
    
    Delay::Delay( std::string name, JsonTree jsonData, vec2 origin, vec2 size, gl::Context * mMainWinCtx ) : ModuleVideo(name+" "+ tools.to_roman(Delay::COUNT), origin, size, 2, 1, true){
        if(jsonData.getNumChildren()!=0){
            data = DATA(jsonData);
        }
        this->mMainWinCtx = mMainWinCtx;
    }
    
    void Delay::setup(){
        ModuleVideo::setup();
        mFbo = gl::Fbo::create(FBO_WIDTH,
                               FBO_HEIGHT,
                               gl::Fbo::Format()
                               .attachment(GL_COLOR_ATTACHMENT0,
                                           gl::Texture2d::create(FBO_WIDTH,
                                                                 FBO_HEIGHT)));
        mFbo->bindFramebuffer();
        gl::clear(Color::black());
        mFbo->unbindFramebuffer();
        mFbo2 = gl::Fbo::create(FBO_WIDTH,
                                FBO_HEIGHT,
                                gl::Fbo::Format()
                                .attachment(GL_COLOR_ATTACHMENT0,
                                            gl::Texture2d::create(FBO_WIDTH,
                                                                  FBO_HEIGHT)));
        mFbo2->bindFramebuffer();
        gl::clear(Color::black());
        mFbo2->unbindFramebuffer();
        
        outputs['A'] = mFbo2->getColorTexture();
        
        
        for(int i = 0 ; i < MAX_TEX ; i ++){
            mFboBuffer.push_back(gl::Fbo::create(MAX_IMG_PER_TEX*FBO_WIDTH,
                                                 FBO_HEIGHT,
                                                 gl::Fbo::Format()
                                                 .attachment(GL_COLOR_ATTACHMENT0,
                                                             gl::Texture2d::create(MAX_IMG_PER_TEX*FBO_WIDTH,
                                                                                   FBO_HEIGHT))));
            mFboBuffer.back()->bindFramebuffer();
            gl::clear(Color::black());
            mFboBuffer.back()->unbindFramebuffer();
        }
        
        setupShader();
        setupUI();
        
        // allocate our UBO
        dataUbo = gl::Ubo::create( sizeof( data ), &data, GL_DYNAMIC_DRAW );
        // and bind it to buffer base 0; this is analogous to binding it to texture unit 0
        dataUbo->bindBufferBase( id );
        // and finally tell the shaders that their uniform buffer 'FormulaParams' can be found at buffer base 0
        mShader->uniformBlock("delay", id );
    }
    
    void Delay::update(){
        
        if(mMainWinCtx != gl::Context::getCurrent()){
            return;
        }
        ModuleVideo::update();
        
        if(inputs['A']){
            gl::pushMatrices();
            gl::ScopedViewport scpVp( ivec2( 0 ), vec2(MAX_IMG_PER_TEX*FBO_WIDTH, FBO_HEIGHT) );
            gl::setMatrices( CAM2 );
            
            
            int y = (int)(data.cursor/MAX_IMG_PER_TEX);
            int x = (int)(data.cursor%MAX_IMG_PER_TEX);
            gl::FboRef current = mFboBuffer.at(y);
            
            current->bindFramebuffer();
            gl::color(Color::black());
            Area area = Area( x*FBO_WIDTH, 0, (x+1)*FBO_WIDTH, FBO_HEIGHT);
            gl::drawSolidRect(area);
            gl::color(Color::white());
            gl::draw(inputs['A'], area);
            current->unbindFramebuffer();
            gl::popMatrices();
        }

        gl::pushMatrices();
        gl::ScopedViewport scpVp( ivec2( 0 ), mFbo->getSize() );
        gl::setMatrices( ModuleVideo::CAM );
            
        mFbo->bindFramebuffer();
        {
            gl::clear( ColorA(0, 0, 0, 0));
            dataUbo->bufferSubData( 0, sizeof( data ), &data );
            gl::ScopedGlslProg glslProg( mShader );
            
            
            for(int i = 0 ; i < MAX_TEX ; i ++){
                mFboBuffer.at(i)->getColorTexture()->bind(i);
                //testTex->bind(0);
                mShader->uniform( "tex["+to_string(i)+"]", i );  // texunit 0
            }
            if(inputs['B']){
                inputs['B']->bind(MAX_TEX);
                mShader->uniform( "modifier", MAX_TEX );  // texunit 0
                data.modifierActive = true;
            }else{
                data.modifierActive = false;
            }
            
            gl::enableAlphaBlendingPremult();
            gl::color(Color::white());
            gl::drawSolidRect(Area( 0, 0, mFbo->getWidth(), mFbo->getHeight() ));
            
            for(int i = 0 ; i < MAX_TEX ; i ++){
                mFboBuffer.at(i)->getColorTexture()->unbind(i);
            }
            if(inputs['B']){
                inputs['B']->unbind(MAX_TEX);
            }
        }
        mFbo->unbindFramebuffer();
        
        mFbo2->bindFramebuffer();
        {
            gl::enableAlphaBlendingPremult();
            gl::color( ColorA(1, 1, 1, 1));
            gl::draw(mFbo->getColorTexture(), Area(0, 0, mFbo2->getWidth(), mFbo2->getHeight()));
        }
        mFbo2->unbindFramebuffer();
        
        gl::popMatrices();
        
        
        data.bufferLen = int(bufferLen);
        data.cursor += 1;
        data.cursor %= data.bufferLen;
        
    }
    
    void Delay::setupUI(){
        ModuleVideo::setupUI();
        mUi->setColorBack(ColorAT<float>(vec4(.1f, .7f, .3f, .4f)));
        //mUi->setColorFill(ColorAT<float>(vec4(.8f, .9f, 1.f, .6f)));
        mUi->setColorFillHighlight(ColorAT<float>(vec4(.3f, .9f, 1.f, 1.f)));
        
        tools.addSlider(mUi, "Length", this->id, &(bufferLen), 1, MAX_BUFFER);
        /*
            BTN RESET
            BTN RECORD
            SEEKER (SLIDER)
            SPEED
          */
        
        mUi->setMinified(true);
    }
    
    void Delay::setupShader(){
        ModuleVideo::setupShader();
        mShader = gl::GlslProg::create(
                                       loadAsset( "shaders/delay.vert"),
                                       loadAsset( "shaders/delay.frag")
                                       );
    }
}
