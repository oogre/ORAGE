//
//  DynamicAnalyser.cpp
//  PianoProjete
//
//  Created by Vincent Evrard on 2017-05-29.
//
//

#include "DynamicAnalyser.h"


using namespace ogre;
using namespace cinder;
using namespace cinder::gl;
using namespace std;
using namespace reza::ui;

namespace ogre {
    
    int DynamicAnalyser::COUNT = 0;
    
    DynamicAnalyser::DynamicAnalyser( std::string name, vec2 origin, vec2 size, gl::Context * mMainWinCtx ) : ModulePiano(name+" "+ tools.to_roman(DynamicAnalyser::COUNT), origin, size, 1, 0){
        this->mMainWinCtx = mMainWinCtx;
    }
    
    void DynamicAnalyser::setup(){
        ModulePiano::setupUI();
        setupUI();
    }
    
    
    void DynamicAnalyser::update(){
        
        if(mMainWinCtx != gl::Context::getCurrent()){
            return;
        }
        ModulePiano::update();
        
        if(inputs['A'] && inputs['A']->length!=0){
            auto it = inputs['A']->notes.begin();
            auto end = inputs['A']->notes.end();
            int p = 0;
            float t = 0.f;
            float tm = 0.f;
            int c = 0;
            int cm = 0;
            for(; it != end ; it++){
                p += (*it)->num;
                t += (*it)->time();// * (*it)->norm; // => PLus une Note est aigue plus l'horloge bat vite
                tm += (*it)->time(true);// * (*it)->norm; // => PLus une Note est aigue plus l'horloge bat vite
                c += (*it)->playedCount;
                cm += (*it)->playedCountMem;
            }
            oldPosition = positionRate;
            positionRate = (float)p / (float)inputs['A']->length;
            timeRate = fract(t*0.01f);
            timeMemRate = fract(tm*0.01f);
            countRate = int(c *0.1f) % 10;
            countMemRate = int(cm*0.1f) % 10;
            
            if(t != 0){
                speedRate = max(0.0f, min((float)cm / (float)tm, (float)Config::MAX_SINGLE_NOTE_SPEED));
            }
            if(oldInputID != inputs['A']->ID){
                oldInputID = inputs['A']->ID;
                distRate = abs(positionRate - oldPosition);
            }
            delay = getElapsedSeconds();
        }else{
            if(getElapsedSeconds() - delay > 0.5f){
                positionRate = 0.f;
                timeRate =  0.f;
                timeMemRate =  0.f;
                countRate =  0.f;
                countMemRate =  0.f;
                speedRate = 0.0f;
                distRate = 0.0f;
            }
        }
        
        position += (positionRate - position) * inertie;
        time += (timeRate - time) * inertie;
        timeMem += (timeMemRate - timeMem) * inertie;
        count += (countRate - count) * inertie;
        countMem += (countMemRate - countMem) * inertie;
        speed += (speedRate - speed) * inertie;
        dist += (distRate - dist) * inertie;
        
    }
    
    void DynamicAnalyser::setupUI(){
        mUi->setColorBack(ColorAT<float>(vec4(.1f, .2f, .3f, .4f)));
        //mUi->setColorFill(ColorAT<float>(vec4(.8f, .9f, 1.f, .6f)));
        mUi->setColorFillHighlight(ColorAT<float>(vec4(.1f, .9f, 1.f, 1.f)));
        
        ModulePiano::setupUI();
        setupInput();
        setupOutput();
        
        tools.addSlider(mUi, "INERTIE", this->id, &inertie, 0, 1);
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        
        tools.addSlider(mUi, "Position", this->id, &position, 0, Config::NOTE_LEN);
        tools.addSlider(mUi, "Dist", this->id, &dist, 0, Config::NOTE_LEN);
        
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        
        tools.addSlider(mUi, "Count", this->id, &count, 0, 10);
        tools.addSlider(mUi, "Count Mem", this->id, &countMem, 0, 10);
        
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        
        tools.addSlider(mUi, "Time", this->id, &time, 0, 1);
        tools.addSlider(mUi, "Time Mem", this->id, &timeMem, 0, 1);
        
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        
        tools.addSlider(mUi, "Speed", this->id, &speed, 0, Config::MAX_SINGLE_NOTE_SPEED);
        
        mUi->setMinified(false);
    }
};
