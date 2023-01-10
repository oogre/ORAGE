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
    Random::Random( std::string name, JsonTree jsonData, vec2 origin, vec2 size, gl::Context * mMainWinCtx ) : ModuleCommon(name+" "+ tools.to_roman(Random::COUNT), origin, size){
        if(jsonData.getNumChildren()!=0){
            data = DATA(jsonData);
        }
        this->mMainWinCtx = mMainWinCtx;
        this->typeName = "Random";
    }
    
    void Random::setup(){
        ModuleCommon::setup();
        setupUI();
        p.setSeed(data.seed);
    }
    
    void Random::update(){
        if(mMainWinCtx != gl::Context::getCurrent()){
            return;
        }
        ModuleCommon::update();
        float dist = data.input - data.oldInput;
        if(abs(dist) > 0.5) dist = 1 - abs(dist);
        data.counter += dist;
        data.oldInput = data.input;
        
        float multi = 0;
        if(data.randomness >= 0.5 ){
            multi = lerp(1, 10, (data.randomness - 0.5)*2);
        }else{
            multi = data.randomness * 2;
        }
        data.linear = fmod( multi * (p.noise(data.counter) + 1) * 0.5f , 1.0f);
    }
    
    void Random::setupUI(){
        ModuleCommon::setupUI();
        mUi->setColorBack(ColorAT<float>(vec4(.1f, .2f, .3f, .4f)));
        //mUi->setColorFill(ColorAT<float>(vec4(.8f, .9f, 1.f, .6f)));
        mUi->setColorFillHighlight(ColorAT<float>(vec4(.1f, .9f, 1.f, 1.f)));
        
        string id = toString(COUNT);
        CanvasRef v = Canvas::create("RANDOM"+id, ci::app::getWindow());
        
        tools.addSlider(mUi, "input "+id, this->id, &(data.input), 0, 1, 0, true);
        tools.addSlider(mUi, "rand "+id, this->id, &(data.randomness ), 0, 1, 0, true);
        tools.addSlider(mUi, "linear "+id, this->id, &(data.linear), 0, 1, 0, true);
        
        mUi->setMinified(false);
    }
}
