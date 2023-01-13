//
//  Rotate.cpp
//  PianoProjete
//
//  Created by Vincent Evrard on 2017-06-10.
//
//

#include "Rotate.h"


using namespace ogre;
using namespace cinder;
using namespace cinder::gl;
using namespace std;
using namespace reza::ui;

namespace ogre {
    
    int Rotate::COUNT = 0;
    
    
    Rotate::Rotate( std::string name, JsonTree jsonData, vec2 origin, vec2 size, gl::Context * mMainWinCtx ) : ModuleVideo(name+" "+ tools.to_roman(Rotate::COUNT), origin, size, 2, 1, true){
        if(jsonData.getNumChildren()!=0){
            data = DATA(jsonData);
        }
        this->mMainWinCtx = mMainWinCtx;
    }
    
    void Rotate::setup(){
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
    
    void Rotate::update(){
        
        if(mMainWinCtx != gl::Context::getCurrent()){
            return;
        }
        ModuleVideo::update();
        gl::pushMatrices();
        {
            gl::ScopedViewport scpVp( ivec2( 0 ), mFbo->getSize() );
            gl::setMatrices( ModuleVideo::CAM );
            mFbo2->bindFramebuffer();
            {
                gl::clear( ColorA(0, 0, 0, 1.0f));
                gl::color( ColorA(1.0f,1.0f,1.0f,1.0f));
                if(inputs['B']){
                    gl::draw(inputs['B'], Area(0, 0, FBO_WIDTH, FBO_HEIGHT), Area(0, 0, FBO_WIDTH, FBO_HEIGHT));
                }
                if(inputs['A']){
                    float _y = FBO_HEIGHT * data.posY.value;
                    float _x = FBO_WIDTH * data.posX.value;
                    float _z = FBO_HEIGHT * data.posZ.value;
                    gl::pushMatrices();
                    gl::translate( vec3(_x, _y , _z) );
                    gl::rotate(data.rX.value, 1, 0, 0);
                    gl::rotate(data.rY.value, 0, 1, 0);
                    gl::rotate(data.rZ.value, 0, 0, 1);
                    gl::translate( vec3(-1.f * _x, -1.f * _y , -1.f * _z) );
                    gl::draw(inputs['A'], Area(0, 0, FBO_WIDTH, FBO_HEIGHT), Area(0, 0, FBO_WIDTH, FBO_HEIGHT));
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
        }
        gl::popMatrices();
    }
    
    void Rotate::setupUI(){
        ModuleVideo::setupUI();
        mUi->setColorBack(ColorAT<float>(vec4(.1f, .7f, .3f, .4f)));
        //mUi->setColorFill(ColorAT<float>(vec4(.8f, .9f, 1.f, .6f)));
        mUi->setColorFillHighlight(ColorAT<float>(vec4(.3f, .9f, 1.f, 1.f)));
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        mUi->addLabel("Rotation");
        tools.addSlider(mUi, "rX", this->id, &(data.rX));
        tools.addSlider(mUi, "rY", this->id, &(data.rY));
        tools.addSlider(mUi, "rZ", this->id, &(data.rZ));
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        mUi->addLabel("Translation");
        tools.addSlider(mUi, "tX", this->id, &(data.posX));
        tools.addSlider(mUi, "tY", this->id, &(data.posY));
        tools.addSlider(mUi, "tZ", this->id, &(data.posZ));
        mUi->setMinified(false);
    }
    
    void Rotate::setupShader(){
        
        ModuleVideo::setupShader();
    }
}
