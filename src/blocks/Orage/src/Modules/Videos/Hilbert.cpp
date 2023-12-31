//
//  Hilbert.cpp
//  orage
//
//  Created by Vincent Evrard on 2017-04-15.
//
//

#include "Hilbert.h"


using namespace ogre;
using namespace cinder;
using namespace cinder::gl;
using namespace std;
using namespace reza::ui;

namespace ogre {
    
int Hilbert::COUNT = 0;

Hilbert::Hilbert( std::string name, JsonTree jsonData, vec2 origin, vec2 size, gl::Context * mMainWinCtx ) : ModuleVideo(name+" "+ tools.to_roman(Hilbert::COUNT), origin, size, 1, 1, true){
    if(jsonData.getNumChildren()!=0){
        data = DATA(jsonData);
        trail = jsonData.getChild("trail").getValue<float>();
    }
    this->mMainWinCtx = mMainWinCtx;
}

void Hilbert::setup(){
    ModuleVideo::setup();
    mFbo = gl::Fbo::create(FBO_WIDTH,
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
    mShader->uniformBlock("oscillator", id );
    
    
}

void Hilbert::update(){
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
        
        gl::color(Color::white());
        gl::drawSolidRect(Area( 0, 0, mFbo->getWidth(), mFbo->getHeight() ));
        
        if(inputs['A']){
            inputs['A']->unbind(0);
        }
    }
    mFbo->unbindFramebuffer();

    gl::popMatrices();
}

void Hilbert::setupUI(){
    
    ModuleVideo::setupUI();
    mUi->setColorBack(ColorAT<float>(vec4(.1f, .8f, .8f, .4f)));
    //mUi->setColorFill(ColorAT<float>(vec4(.8f, .9f, 1.f, .6f)));
    mUi->setColorFillHighlight(ColorAT<float>(vec4(.1f, .9f, 1.f, 1.f)));
    
    mUi->addSpacer(false);
    mUi->addSpacer(false);
    
    mUi->setMinified(false);
}

void Hilbert::setupShader(){
        ModuleVideo::setupShader();
        mShader = gl::GlslProg::create(
                                       loadAsset( "shaders/hilbert.vert"),
                                       loadAsset( "shaders/hilbert.frag")
                                       );
    }
}
