//
//  Cloud.cpp
//  ORAGE2
//
//  Created by Vincent Evrard on 2017-04-27.
//
//

#include "Cloud.h"




using namespace ogre;
using namespace cinder;
using namespace cinder::gl;
using namespace std;
using namespace reza::ui;

namespace ogre {
    
    int Cloud::COUNT = 0;
    
    Cloud::Cloud( std::string name, JsonTree jsonData, vec2 origin, vec2 size, gl::Context * mMainWinCtx ) : ModuleVideo(name+" "+ tools.to_roman(Cloud::COUNT), origin, size, 2, 2, true){
        if(jsonData.getNumChildren()!=0){
            data = DATA(jsonData);
        }
        this->mMainWinCtx = mMainWinCtx;
    }
    
    void Cloud::setup(){
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
        
        // allocate our UBO
        dataUbo = gl::Ubo::create( sizeof( sData ), &sData, GL_DYNAMIC_DRAW );
        // and bind it to buffer base 0; this is analogous to binding it to texture unit 0
        dataUbo->bindBufferBase( id );
        // and finally tell the shaders that their uniform buffer 'FormulaParams' can be found at buffer base 0
        mShader->uniformBlock("cloud", id );
        
    }
    
    void Cloud::update(){
        if(mMainWinCtx != gl::Context::getCurrent()){
            return;
        }
        ModuleVideo::update();
        float now = getElapsedSeconds();
        sData.u_time += 10 * data.move.value * data.move.value * data.move.value * (o_time - now);
        o_time = now;
        sData.octave = data.octave.value;
        sData.sizex = data.sizex.value;
        sData.sizey = data.sizey.value;
        sData.move = data.move.value;
        sData.redShift = data.redShift.value;
        sData.blueShift = data.blueShift.value;
        sData.greenShift = data.greenShift.value;
        sData.redLvl = data.redLvl.value;
        sData.blueLvl = data.blueLvl.value;
        sData.greenLvl = data.greenLvl.value;
        sData.lvl = data.lvl.value;
        
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
                sData.modifierA = 1;
            }else{
                sData.modifierA = 0;
            }
            if(inputs['B']){
                inputs['B']->bind(1);
                mShader->uniform( "tex1", 1 );  // texunit 0
                sData.modifierB = 1;
            }else{
                sData.modifierB = 0;
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
        
        gl::popMatrices();
    }
    
    void Cloud::setupUI(){
        
        ModuleVideo::setupUI();
        mUi->setColorBack(ColorAT<float>(vec4(.8f, .4f, .1f, .4f)));
        //mUi->setColorFill(ColorAT<float>(vec4(.8f, .9f, 1.f, .6f)));
        mUi->setColorFillHighlight(ColorAT<float>(vec4(.1f, .9f, 1.f, 1.f)));
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        
        
        tools.addSlider(mUi, "octave", this->id, &(data.octave));
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        tools.addSlider(mUi, "sizex", this->id, &(data.sizex));
        tools.addSlider(mUi, "sizey", this->id, &(data.sizey));
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        tools.addSlider(mUi, "move", this->id, &(data.move));
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        
        tools.addSlider(mUi, "redShift", this->id, &(data.redShift));
        tools.addSlider(mUi, "blueShift", this->id, &(data.blueShift));
        tools.addSlider(mUi, "greenShift", this->id, &(data.greenShift));
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        
        tools.addSlider(mUi, "redLvl", this->id, &(data.redLvl));
        tools.addSlider(mUi, "blueLvl", this->id, &(data.blueLvl));
        tools.addSlider(mUi, "greenLvl", this->id, &(data.greenLvl));
        tools.addSlider(mUi, "lvl", this->id, &(data.lvl));
    
        mUi->setMinified(false);
    }
    
    void Cloud::setupShader(){
        ModuleVideo::setupShader();
        mShader = gl::GlslProg::create(
                                       loadAsset( "shaders/cloud.vert"),
                                       loadAsset( "shaders/cloud.frag")
                                       );
    }
    
}
