//
//  ColorAdjustement.cpp
//  orage
//
//  Created by Vincent Evrard on 2017-04-23.
//
//

#include "ColorAdjustement.h"


using namespace ogre;
using namespace cinder;
using namespace cinder::gl;
using namespace std;
using namespace reza::ui;

namespace ogre {
    
    int ColorAdjustement::COUNT = 0;
    
    ColorAdjustement::ColorAdjustement( std::string name, JsonTree jsonData, vec2 origin, vec2 size, gl::Context * mMainWinCtx ) : ModuleVideo(name+" "+ tools.to_roman(ColorAdjustement::COUNT), origin, size, 2, 1, true){
        if(jsonData.getNumChildren()!=0){
            data = DATA(jsonData);
        }
        this->mMainWinCtx = mMainWinCtx;
    }
    
    void ColorAdjustement::setup(){
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
        mShader->uniformBlock("coloradjustement", id );
        
        
    }
    
    void ColorAdjustement::update(){
        
        if(mMainWinCtx != gl::Context::getCurrent()){
            return;
        }
        ModuleVideo::update();
        
        sData.bri = data.bri.value;
        sData.sat = data.sat.value;
        sData.con = data.con.value;
        sData.red = data.red.value;
        sData.green = data.green.value;
        sData.blue = data.blue.value;
        sData.ssm = data.ssm.value;
        sData.ssM = data.ssM.value;
        
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
                mShader->uniform( "tex1", 1 );  // texunit 0
                sData.modifier = 1;
            }else{
                sData.modifier = 0;
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
            gl::color( ColorA(1, 1, 1, 1));
            gl::draw(mFbo->getColorTexture(), Area(0, 0, mFbo2->getWidth(), mFbo2->getHeight()));
        }
        mFbo2->unbindFramebuffer();
        gl::popMatrices();
    }
    
    void ColorAdjustement::setupUI(){
        ModuleVideo::setupUI();
        mUi->setColorBack(ColorAT<float>(vec4(.1f, .7f, .3f, .4f)));
        //mUi->setColorFill(ColorAT<float>(vec4(.8f, .9f, 1.f, .6f)));
        mUi->setColorFillHighlight(ColorAT<float>(vec4(.3f, .9f, 1.f, 1.f)));
        
        tools.addSlider(mUi, "bri", this->id, &(data.bri));
        tools.addSlider(mUi, "sat", this->id, &(data.sat));
        tools.addSlider(mUi, "con", this->id, &(data.con));
        tools.addSlider(mUi, "red", this->id, &(data.red));
        tools.addSlider(mUi, "green", this->id, &(data.green));
        tools.addSlider(mUi, "blue", this->id, &(data.blue));
        tools.addSlider(mUi, "ssm", this->id, &(data.ssm));
        tools.addSlider(mUi, "ssM", this->id, &(data.ssM));
        
        mUi->setMinified(true);
    }
    
    void ColorAdjustement::setupShader(){
        ModuleVideo::setupShader();
        mShader = gl::GlslProg::create(
                                       loadAsset( "shaders/coloradjustement.vert"),
                                       loadAsset( "shaders/coloradjustement.frag")
                                       );
    }
}
