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
    
    Resize::Resize( std::string name, JsonTree jsonData, vec2 origin, vec2 size, gl::Context * mMainWinCtx ) : ModuleVideo(name+" "+ tools.to_roman(Resize::COUNT), origin, size, 1, 1, true){
        if(jsonData.getNumChildren()!=0){
            posX = jsonData.getChild("posX").getValue<float>();
            posY = jsonData.getChild("posY").getValue<float>();
            width = jsonData.getChild("width").getValue<float>();
            height = jsonData.getChild("height").getValue<float>();
            resX = jsonData.getChild("resX").getValue<float>();
            resY = jsonData.getChild("resY").getValue<float>();
            resW = jsonData.getChild("resW").getValue<float>();
            resH = jsonData.getChild("resH").getValue<float>();
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
        

        if(inputs['A']){
            mFbo->bindFramebuffer();
            {
                gl::clear( ColorA(0, 0, 0, 1.0f));
                gl::enableAlphaBlendingPremult();
                gl::color( ColorA(1.0f,1.0f,1.0f,1.0f));
                
                float _y = HALF_FBO_HEIGHT * (2 * posY - height + 1);
                float _x = HALF_FBO_WIDTH * (2 * posX - width + 1);
                gl::translate( vec3(_x, _y , 0) );
                gl::scale( vec2(width ,height) );
                
                gl::draw(inputs['A'], Area(resX*FBO_WIDTH, resY*FBO_HEIGHT, resW*FBO_WIDTH, resH*FBO_HEIGHT), Area(0, 0, FBO_WIDTH, FBO_HEIGHT));
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
        tools.addSlider(mUi, "cX", this->id, &(resX), .0f, 1.0f);
        tools.addSlider(mUi, "cY", this->id, &(resY), .0f, 1.0f);
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        tools.addSlider(mUi, "cW", this->id, &(resW), .0f, 1.0f);
        tools.addSlider(mUi, "cH", this->id, &(resH), .0f, 1.0f);
        mUi->addLabel("Size");
        tools.addSlider(mUi, "sX", this->id, &(posX), -1.0f, 1.0f);
        tools.addSlider(mUi, "sY", this->id, &(posY), -1.0f, 1.0f);
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        tools.addSlider(mUi, "sW", this->id, &(width), -1.0f, 1.0f);
        tools.addSlider(mUi, "sH", this->id, &(height), -1.0f, 1.0f);

        mUi->setMinified(false);
    }
    
    void Resize::setupShader(){
        
        ModuleVideo::setupShader();
    }
}
