//
//  Crossfader.cpp
//  orage
//
//  Created by Vincent Evrard on 2017-04-21.
//
//

#include "Crossfader.h"


using namespace ogre;
using namespace cinder;
using namespace cinder::gl;
using namespace std;
using namespace reza::ui;


struct DATA {
    RANGE crossfade;
    RANGE blend;
    DATA():
    crossfade(0.0f, -1.0f, 1.0f),
    blend(0.0f, 0.0f, 13.0f)
    {};
    DATA(JsonTree data):
    crossfade(data.getChild("crossfade")),
    blend(data.getChild("blend"))
    {}
};
DATA data;

struct S_DATA {
    float alpha0;
    float alpha1;
    float blend;
};

namespace ogre {
    
    int Crossfader::COUNT = 0;
    
    Crossfader::Crossfader( std::string name, JsonTree jsonData, vec2 origin, vec2 size, gl::Context * mMainWinCtx ) : ModuleVideo(name+" "+ tools.to_roman(Crossfader::COUNT), origin, size, 2, 1, true){
        if(jsonData.getNumChildren()!=0){
            data = DATA(jsonData);
        }
        this->mMainWinCtx = mMainWinCtx;
    }
    
    void Crossfader::setup(){
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
        
        oldInputs['A'] = inputs['A'];
        oldInputs['B'] = inputs['B'];
        
        setupShader();
        setupUI();
        
        // allocate our UBO
        dataUbo = gl::Ubo::create( sizeof( sData ), &sData, GL_DYNAMIC_DRAW );
        // and bind it to buffer base 0; this is analogous to binding it to texture unit 0
        dataUbo->bindBufferBase( id );
        // and finally tell the shaders that their uniform buffer 'FormulaParams' can be found at buffer base 0
        mShader->uniformBlock("crossfader", id );
    }
    
    void Crossfader::update(){
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

        sData.blend = data.blend.value;
        sData.alpha0 = inputs['A'] ? 1.f - glm::clamp<float>(data.crossfade.value, 0, 1) : 0.0f;;
        sData.alpha1 = inputs['B'] ? 1.f - (-1 * glm::clamp<float>(data.crossfade.value, -1, 0)) : 0.0f;;
        sData.crossfade = data.crossfade.value;
        
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
                mShader->uniform( "tex1", 1 );  // texunit
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
        
        mFbo2->bindFramebuffer();
        {
            gl::enableAlphaBlendingPremult();
            gl::color( 1, 1, 1, 1);
            gl::draw(mFbo->getColorTexture(), Area(0, 0, mFbo2->getWidth(), mFbo2->getHeight()));
        }
        mFbo2->unbindFramebuffer();

        gl::popMatrices();
     
    }
    
    void Crossfader::setupUI(){
        ModuleVideo::setupUI();
        mUi->setColorBack(ColorAT<float>(vec4(.3f, .1f, .9f, .4f)));
        //mUi->setColorFill(ColorAT<float>(vec4(.8f, .9f, 1.f, .6f)));
        mUi->setColorFillHighlight(ColorAT<float>(vec4(.1f, .9f, 1.f, 1.f)));
        
        {
            inputA = TextureView::create("inputA", inputs['A']);

            float w = mUi->getWidth()*0.5 - mUi->mPadding.mLeft - mUi->mPadding.mRight;
            float h = h = ( w * ModuleVideo::RATIO );
            inputA->setSize( vec2( w, h ) );
            mUi->addSubViewDown(inputA);
        }
        {
            inputB = TextureView::create("inputB", inputs['B']);
            float w =  mUi->getWidth()*0.5 - mUi->mPadding.mLeft - mUi->mPadding.mRight;
            float h = h = ( w * ModuleVideo::RATIO );

            inputB->setSize( vec2( w, h ) );
            mUi->addSubViewRight(inputB);
        }

        
        tools.addSlider(mUi, "blend", this->id, &(data.blend.value), data.blend.min, data.blend.max, data.blend.low, data.blend.high, 260);
        tools.addSlider(mUi, "crossfade", this->id, &(data.crossfade.value), data.crossfade.min, data.crossfade.max, data.crossfade.low, data.crossfade.high, 260);
        
        
        mUi->setMinified(true);
    }
    
    void Crossfader::setupShader(){
        ModuleVideo::setupShader();
        mShader = gl::GlslProg::create(
                                       loadAsset( "shaders/crossfader.vert"),
                                       loadAsset( "shaders/crossfader.frag")
                                       );
    }
    
}
