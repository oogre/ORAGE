//
//  Random.cpp
//  PianoProjete
//
//  Created by Vincent Evrard on 19/03/18.
//

#include "Random.hpp"

using namespace ogre;
using namespace cinder;
using namespace cinder::gl;
using namespace std;
using namespace reza::ui;

namespace ogre {
    
    int Random::COUNT = 0;
    Random::Random( std::string name, vec2 origin, vec2 size, gl::Context * mMainWinCtx ) : ModuleCommon(name+" "+ tools.to_roman(Random::COUNT), origin, size){
        this->mMainWinCtx = mMainWinCtx;
    }
    
    void Random::setup(){
        ModuleCommon::setup();
        setupUI();
        p.setSeed(random());
    }
    
    void Random::update(){
        if(mMainWinCtx != gl::Context::getCurrent()){
            return;
        }
        ModuleCommon::update();
        
        randomDatas.linear = (p.noise(getElapsedSeconds()) + 1) * 0.5f;
        
    }
    
    void Random::setupUI(){
        ModuleCommon::setupUI();
        mUi->setColorBack(ColorAT<float>(vec4(.1f, .2f, .3f, .4f)));
        //mUi->setColorFill(ColorAT<float>(vec4(.8f, .9f, 1.f, .6f)));
        mUi->setColorFillHighlight(ColorAT<float>(vec4(.1f, .9f, 1.f, 1.f)));
        
        string id = toString(COUNT);
        CanvasRef v = Canvas::create("LFO"+id, ci::app::getWindow());
        
        
        tools.addSlider(mUi, "input "+id, this->id, &(randomDatas.input), 0, 1, 0, true);
        tools.addSlider(mUi, "linear "+id, this->id, &(randomDatas.linear), 0, 1, 0, true);
        
        
        mUi->setMinified(false);
    }
    
    
    
    
}
