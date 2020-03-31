//
//  Matte.cpp
//  orage
//
//  Created by Vincent Evrard on 2017-04-20.
//
//

#include "Matte.h"


using namespace ogre;
using namespace cinder;
using namespace cinder::gl;
using namespace std;
using namespace reza::ui;

namespace ogre {
    
    int Matte::COUNT = 0;
    
    Matte::Matte( std::string name, JsonTree jsonData, vec2 origin, vec2 size, gl::Context * mMainWinCtx ) : ModuleVideo(name+" "+ tools.to_roman(Matte::COUNT), origin, size, 0, 1, false){
        if(jsonData.getNumChildren()!=0){
            YPbPr.x = jsonData.getChild("sY").getValue<float>();
            YPbPr.y = jsonData.getChild("sPb").getValue<float>();
            YPbPr.z = jsonData.getChild("sPr").getValue<float>();
        }
        this->mMainWinCtx = mMainWinCtx;
    }
    
    void Matte::setup(){
        ModuleVideo::setup();
        mFbo = gl::Fbo::create(FBO_WIDTH,
                               FBO_HEIGHT,
                               gl::Fbo::Format()
                               .attachment(GL_COLOR_ATTACHMENT0,
                                           gl::Texture2d::create(FBO_WIDTH,
                                                                 FBO_HEIGHT)));
        outputs['A'] = mFbo->getColorTexture();
        
        setupUI();
        
    }
    ColorAT<float> Matte::YPbPr2Color(vec3 YPbPr){
        //http://www.equasys.de/colorconversion.html
        return ColorAT<float>(1.f * YPbPr.x + 0.000f * YPbPr.y + 1.574f * YPbPr.z,
                      1.f * YPbPr.x - 0.187f * YPbPr.y - 0.469f * YPbPr.z,
                      1.f * YPbPr.x + 1.856f * YPbPr.y - 0.000f * YPbPr.z);
    };
    
    void Matte::update(){
        if(mMainWinCtx != gl::Context::getCurrent())
            return;
        ModuleVideo::update();
        
        if(YPbPrOld == YPbPr)
            return;
        YPbPrOld = YPbPr;
        
        color = YPbPr2Color(YPbPr);
        color.r = glm::clamp<float>(color.r, 0, 1);
        color.g = glm::clamp<float>(color.g, 0, 1);
        color.b = glm::clamp<float>(color.b, 0, 1);
        ColorA c = ColorA(color.r, color.g, color.b, 1);

        sY->setColorBack(c);
        sPb->setColorBack(c);
        sPr->setColorBack(c);

        gl::pushMatrices();
        gl::ScopedViewport scpVp( ivec2( 0 ), mFbo->getSize() );
        gl::setMatrices( ModuleVideo::CAM );
        mFbo->bindFramebuffer();
        {
            //gl::clear(Color::black());
            gl::color(c);
            gl::drawSolidRect(Area( 0, 0, mFbo->getWidth(), mFbo->getHeight() ));
        }
        mFbo->unbindFramebuffer();
        
        gl::popMatrices();
    }
    
    void Matte::setupUI(){
        ModuleVideo::setupUI();
        mUi->setColorBack(ColorAT<float>(vec4(.4f, .4f, .4f, .4f)));
        //mUi->setColorFill(ColorAT<float>(vec4(.8f, .9f, 1.f, .6f)));
//        mUi->setColorFillHighlight(ColorAT<float>(vec4(.3f, .9f, 1.f, 1.f)));
        
        
        
        sY = tools.addSlider(mUi, "Y", this->id, &(YPbPr.x), .0f, 1.f);
        sPb = tools.addSlider(mUi, "Pb", this->id, &(YPbPr.y), -0.5f, 0.5f);
        sPr = tools.addSlider(mUi, "Pr", this->id, &(YPbPr.z), -0.5f, 0.5f);
       
        mUi->setMinified(false);
    }
    
    
    
}
