//
//  Oscillator.cpp
//  orage
//
//  Created by Vincent Evrard on 2017-04-15.
//
//

#include "Oscillator.h"


using namespace ogre;
using namespace cinder;
using namespace cinder::gl;
using namespace std;
using namespace reza::ui;

namespace ogre {
    
int Oscillator::COUNT = 0;

Oscillator::Oscillator( std::string name, JsonTree jsonData, vec2 origin, vec2 size, gl::Context * mMainWinCtx ) : ModuleVideo(name+" "+ tools.to_roman(Oscillator::COUNT), origin, size, 2, 2, true){
    if(jsonData.getNumChildren()!=0){
        data = DATA(jsonData);
        trail = jsonData.getChild("trail").getValue<float>();
    }
    this->mMainWinCtx = mMainWinCtx;
}

void Oscillator::setup(){
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
    outputs['B'] = mFbo2->getColorTexture();

    setupShader();
    setupUI();
    
    // allocate our UBO
    dataUbo = gl::Ubo::create( sizeof( data ), &data, GL_DYNAMIC_DRAW );
    // and bind it to buffer base 0; this is analogous to binding it to texture unit 0
    dataUbo->bindBufferBase( id );
    // and finally tell the shaders that their uniform buffer 'FormulaParams' can be found at buffer base 0
    mShader->uniformBlock("oscillator", id );
    
    
}

void Oscillator::update(){
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
        if(inputs['A']){
            inputs['A']->bind(0);
            mShader->uniform( "tex0", 0 );  // texunit 0
        }
        
        mShader->uniform( "tex1active", inputs['B']!=nullptr );
        if(inputs['B']){
            inputs['B']->bind(1);
            mShader->uniform( "tex1", 1 );  // texunit
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
        gl::color( ColorA(trail, trail, trail, trail));
        gl::draw(mFbo->getColorTexture(), Area(0, 0, mFbo2->getWidth(), mFbo2->getHeight()));
    }
    mFbo2->unbindFramebuffer();

    gl::popMatrices();
}

void Oscillator::setupUI(){
    
    ModuleVideo::setupUI();
    mUi->setColorBack(ColorAT<float>(vec4(.1f, .8f, .8f, .4f)));
    //mUi->setColorFill(ColorAT<float>(vec4(.8f, .9f, 1.f, .6f)));
    mUi->setColorFillHighlight(ColorAT<float>(vec4(.1f, .9f, 1.f, 1.f)));
    
    mUi->addSpacer(false);
    mUi->addSpacer(false);
    
    tools.addSlider(mUi, "Hz", this->id, &(data.freq), 0.0125f, 600.0f);
    tools.addSlider(mUi, "Sync", this->id, &(data.sync), 0.0f, 1.0f);
    
    mUi->addSpacer(false);
    mUi->addSpacer(false);
    
    
    //mUi->addXYPad("offset", vec2(data.phaseDx, data.phaseDy), XYPad::Format().label(false));
    
    tools.addSlider(mUi, "Dx", this->id, &(data.phaseDx), 0.0f, 1.0f);
    tools.addSlider(mUi, "Dy", this->id, &(data.phaseDy), -1.0f, 1.0f);
    tools.addSlider(mUi, "Mod", this->id, &(data.modifier), 0.0f, 1.0f);
    tools.addSlider(mUi, "Trail", this->id, &(trail), 0.0f, 1.0f);
    
    mUi->addSpacer(false);
    mUi->addSpacer(false);
    
    tools.addSlider(mUi, "Sine", this->id, &(data.sine), 0.0f, 1.0f);
    tools.addSlider(mUi, "Rect", this->id, &(data.rect), 0.0f, 1.0f);
    tools.addSlider(mUi, "Saw", this->id, &(data.saw), 0.0f, 1.0f);
    tools.addSlider(mUi, "Tri", this->id, &(data.tri), 0.0f, 1.0f);
    tools.addSlider(mUi, "Noise", this->id, &(data.noise), 0.0f, 1.0f);
    tools.addSlider(mUi, "Exp", this->id, &(data.exp), 0.064f, 10.0f);
    
    mUi->addSpacer(false);
    mUi->addSpacer(false);
    
    mUi->addToggle("Reverse", &data.reverse);

    mUi->setMinified(true);
}

    void Oscillator::setupShader(){
        ModuleVideo::setupShader();
        mShader = gl::GlslProg::create(
                                       loadAsset( "shaders/oscillator.vert"),
                                       loadAsset( "shaders/oscillator.frag")
                                       );
    }
}
