//
//  Lfos.cpp
//  orage
//
//  Created by Vincent Evrard on 2017-04-22.
//
//

#include "Lfos.h"

using namespace ogre;
using namespace cinder;
using namespace cinder::gl;
using namespace std;
using namespace reza::ui;

namespace ogre {
    
    int Lfos::COUNT = 0;
    
    
    Lfos::Lfos( std::string name, JsonTree jsonData,  vec2 origin, vec2 size, gl::Context * mMainWinCtx ) : ModuleCommon(name+" "+ tools.to_roman(Lfos::COUNT), origin, size){
        if(jsonData.getNumChildren()!=0){
            data = DATA(jsonData);
        }
        this->mMainWinCtx = mMainWinCtx;
        this->typeName = "Lfos";
    }
    
    void Lfos::setup(){
        ModuleCommon::setup();
        setupUI();
        
    }
    
    void Lfos::update(){
        if(mMainWinCtx != gl::Context::getCurrent()){
            return;
        }
        ModuleCommon::update();

        float timer;
        if(data.active){
            
            mUi->setColorBack(ColorAT<float>(vec4(data.saw, data.saw, data.saw, .4f)));
            
            timer = (getElapsedSeconds()  - oldTime ) * data.BPM * 0.0166667f;
            oldTime = getElapsedSeconds();
            
            float div = 1.f / pow(2, (int)data.clk_div);
            float multi = pow(2, (int)data.clk_multi);
            
            if(data.reverse){
                timer = - timer;
            }
            //<<(timer * div * multi)<<endl;
            timeCounter += timer * div * multi;
            timeCounter  = timeCounter - floor(timeCounter);
        
            float value = timeCounter ;
            value += data.delay;
            value  = value - floor(value);
            float exp = 1.0f;
            if(data.exp >= 5){
                exp = 1.0f / (data.exp - 4.0f);
            }else{
                exp = abs(6.0f - data.exp);
            }
            value = pow(value, exp);
        
            data.saw = value;
        }
        data.sine = cos(data.saw * 6.28);
        data.rect = data.saw > .5;
        data.BPM = round(data.BPM);
    }
    
    void Lfos::setupUI(){
        ModuleCommon::setupUI();
        mUi->setColorBack(ColorAT<float>(vec4(.1f, .2f, .3f, .4f)));
        //mUi->setColorFill(ColorAT<float>(vec4(.8f, .9f, 1.f, .6f)));
        mUi->setColorFillHighlight(ColorAT<float>(vec4(.1f, .9f, 1.f, 1.f)));
        
        string id = toString(COUNT);
        CanvasRef v = Canvas::create("LFO"+id, ci::app::getWindow());
        
        ToggleRef on = Toggle::create("ON "+id, &(data.active), Button::Format().label(true));
        ToggleRef rev = Toggle::create("<-> "+id, &(data.reverse), Button::Format().label(true));
        
        mUi->addSubViewDown(on);
        mUi->addSubViewToHeader(on);
        mUi->addSubViewEastOf(rev, "ON "+id);
        mUi->addSubViewToHeader(rev);
        
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        
        tools.addSlider(mUi, "saw "+id, this->id, &(data.saw), 0, 1, 0, true);
        tools.addSlider(mUi, "sine "+id, this->id, &(data.sine), -1, 1, 0, true);
        tools.addSlider(mUi, "rect "+id, this->id, &(data.rect), 0, 1, 0, true);
        
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        
        tools.addSlider(mUi, "BPM "+id, this->id, &(data.BPM), 30.f, 240.0f, 0, true);
        
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        
        tools.addSlider(mUi, "multi "+id, this->id, &(data.clk_multi), 0.0f, 8.0f, 0, true);
        tools.addSlider(mUi, "div "+id, this->id, &(data.clk_div), 0.0f, 8.0f, 0, true);
        
        tools.addSlider(mUi, "delay "+id, this->id, &(data.delay), 0.0f, 1.0f, 0, true);
        tools.addSlider(mUi, "exp "+id, this->id, &(data.exp), 1.0f, 9.0f, 0, true);
            
        
        
        mUi->setMinified(false);
    }
    
    
    
    
}
