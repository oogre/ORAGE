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
    float Rotate::TWO_PI = 6.283185306f;
    
    Rotate::Rotate( std::string name, JsonTree jsonData, vec2 origin, vec2 size, gl::Context * mMainWinCtx ) : ModuleVideo(name+" "+ tools.to_roman(Rotate::COUNT), origin, size, 2, 1, true){
        if(jsonData.getNumChildren()!=0){
            posX = jsonData.getChild("posX").getValue<float>();
            posY = jsonData.getChild("posY").getValue<float>();
            posZ = jsonData.getChild("posZ").getValue<float>();
            rX = jsonData.getChild("rX").getValue<float>();
            rY = jsonData.getChild("rY").getValue<float>();
            rZ = jsonData.getChild("rZ").getValue<float>();
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
                float _y = HALF_FBO_HEIGHT * posY;
                float _x = HALF_FBO_WIDTH * posX;
                float _z = HALF_FBO_HEIGHT * posZ;
                gl::pushMatrices();
                gl::translate( vec3(_x, _y , _z) );
                gl::rotate(rX, 1, 0, 0);
                gl::rotate(rY, 0, 1, 0);
                gl::rotate(rZ, 0, 1, 1);
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
        tools.addSlider(mUi, "rX", this->id, &(rX), 0, TWO_PI);
        tools.addSlider(mUi, "rY", this->id, &(rY), 0, TWO_PI);
        tools.addSlider(mUi, "rZ", this->id, &(rZ), 0, TWO_PI);
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        mUi->addLabel("Translation");
        tools.addSlider(mUi, "tX", this->id, &(posX), .0f, 1.0f);
        tools.addSlider(mUi, "tY", this->id, &(posY), .0f, 1.0f);
        tools.addSlider(mUi, "tZ", this->id, &(posZ), .0f, 1.0f);
        mUi->setMinified(false);
    }
    
    void Rotate::setupShader(){
        
        ModuleVideo::setupShader();
    }
}
