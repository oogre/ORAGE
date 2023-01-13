//
//  Resize.cpp
//  PianoProjete
//
//  Created by Vincent Evrard on 2017-06-10.
//
//

#include "Resize.h"


using namespace ogre;
using namespace cinder;
using namespace cinder::gl;
using namespace std;
using namespace reza::ui;

namespace ogre {
    
    int Resize::COUNT = 0;
    
    Resize::Resize( std::string name, JsonTree jsonData, vec2 origin, vec2 size, gl::Context * mMainWinCtx ) : ModuleVideo(name+" "+ tools.to_roman(Resize::COUNT), origin, size, 2, 1, true){
        if(jsonData.getNumChildren()!=0){
            data = DATA(jsonData);
        }
        this->mMainWinCtx = mMainWinCtx;
    }
    
    void Resize::setup(){
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
        
        
    }
    
    void Resize::update(){
        
        if(mMainWinCtx != gl::Context::getCurrent()){
            return;
        }
        ModuleVideo::update();
        
        gl::pushMatrices();
        
        gl::ScopedViewport scpVp( ivec2( 0 ), mFbo->getSize() );
        gl::setMatrices( ModuleVideo::CAM );
        

        
        mFbo2->bindFramebuffer();
        {
            gl::clear( ColorA(0, 0, 0, 1.0f));
//                gl::enableAlphaBlendingPremult();
            gl::color( ColorA(1.0f,1.0f,1.0f,1.0f));
            if(inputs['B']){
                gl::draw(inputs['B'], Area(0, 0, FBO_WIDTH, FBO_HEIGHT), Area(0, 0, FBO_WIDTH, FBO_HEIGHT));
            }
            if(inputs['A']){
                float _y = HALF_FBO_HEIGHT * (2 * data.posY.value - data.height.value + 1);
                float _x = HALF_FBO_WIDTH * (2 * data.posX.value - data.width.value + 1);
                gl::pushMatrices();
                gl::translate( vec3(_x, _y , 0) );
                gl::scale( vec2(data.width.value, data.height.value) );
                gl::draw(inputs['A'], Area(data.resX.value*FBO_WIDTH, data.resY.value*FBO_HEIGHT, data.resW.value*FBO_WIDTH, data.resH.value*FBO_HEIGHT), Area(0, 0, FBO_WIDTH, FBO_HEIGHT));
                gl::popMatrices();
            }
        mFbo2->unbindFramebuffer();
        
        mFbo->bindFramebuffer();
        {
            gl::clear( ColorA(0, 0, 0, 1.0f));
            gl::draw(mFbo2->getColorTexture(), Area(0, 0, FBO_WIDTH, FBO_HEIGHT), Area(0, 0, FBO_WIDTH, FBO_HEIGHT));
        }
        mFbo->unbindFramebuffer();
            
        }
        
        gl::popMatrices();
    }
    
    void Resize::setupUI(){
        ModuleVideo::setupUI();
        mUi->setColorBack(ColorAT<float>(vec4(.1f, .7f, .3f, .4f)));
        //mUi->setColorFill(ColorAT<float>(vec4(.8f, .9f, 1.f, .6f)));
        mUi->setColorFillHighlight(ColorAT<float>(vec4(.3f, .9f, 1.f, 1.f)));
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        mUi->addLabel("Crope");
        tools.addSlider(mUi, "cX", this->id, &(data.resX));
        tools.addSlider(mUi, "cY", this->id, &(data.resY));
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        tools.addSlider(mUi, "cW", this->id, &(data.resW));
        tools.addSlider(mUi, "cH", this->id, &(data.resH));
        mUi->addLabel("Size");
        tools.addSlider(mUi, "sX", this->id, &(data.posX));
        tools.addSlider(mUi, "sY", this->id, &(data.posY));
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        tools.addSlider(mUi, "sW", this->id, &(data.width));
        tools.addSlider(mUi, "sH", this->id, &(data.height));

        mUi->setMinified(false);
    }
    
    void Resize::setupShader(){
        
        ModuleVideo::setupShader();
    }
}
