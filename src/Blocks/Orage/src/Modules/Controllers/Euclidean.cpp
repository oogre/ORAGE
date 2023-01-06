//
//  Euclidean.cpp
//  orage
//
//  Created by Vincent Evrard on 2017-04-22.
//
//

#include "Euclidean.h"

using namespace ogre;
using namespace cinder;
using namespace cinder::gl;
using namespace std;
using namespace reza::ui;

namespace ogre {
    
    int Euclidean::COUNT = 0;
    
    
    Euclidean::Euclidean( std::string name, JsonTree jsonData,  vec2 origin, vec2 size, gl::Context * mMainWinCtx ) : ModuleCommon(name+" "+ tools.to_roman(Euclidean::COUNT), origin, size){
        if(jsonData.getNumChildren()!=0){
            data = DATA(jsonData);
        }
        this->mMainWinCtx = mMainWinCtx;
        this->typeName = "Euclidean";
     
    }
    
    void Euclidean::setup(){
        ModuleCommon::setup();
        setupUI();
    }
    
    void Euclidean::update(){
        if(mMainWinCtx != gl::Context::getCurrent()){
            return;
        }
        ModuleCommon::update();
        
        int steps = floor(data.steps);
        int beats = floor(data.beats);
        
        data.steps = steps;
        data.beats = min(steps, beats);
        
        if(oldSteps != steps || oldBeats != beats || oldPhase != data.phase){
            rythm = buildRythm(steps, beats, data.phase);
            oldSteps = steps;
            oldBeats = beats;
            oldPhase = data.phase;
        }
        
        data.time = round(data.time);
        if(data.time == 0 && oldTime == 1){
            stepCursor ++;
            stepCursor %= (int)data.steps;
        }
        
        data.click = oldStepCursor == stepCursor ? 1 : 0;
       
        auto rythm_front = rythm.begin();
        std::advance(rythm_front, stepCursor);
        data.out1 = (*rythm_front) ? 1 : 0;
        data.out2 = 1 - data.out1;
    
        oldStepCursor =stepCursor;
        oldTime = data.time;
    }
    
    void Euclidean::setupUI(){
        ModuleCommon::setupUI();
        mUi->setColorBack(ColorAT<float>(vec4(.1f, .2f, .3f, .4f)));
        //mUi->setColorFill(ColorAT<float>(vec4(.8f, .9f, 1.f, .6f)));
        mUi->setColorFillHighlight(ColorAT<float>(vec4(.1f, .9f, 1.f, 1.f)));
        
        string id = toString(COUNT);
        CanvasRef v = Canvas::create("Euclidean"+id, ci::app::getWindow());
        
        
        
        tools.addSlider(mUi, "time "+id, this->id, &(data.time), 0, 1, 0, true);
        tools.addSlider(mUi, "steps "+id, this->id, &(data.steps), 1, Euclidean::maxRythmLength, 0, true);
        tools.addSlider(mUi, "beats "+id, this->id, &(data.beats), 1, Euclidean::maxRythmLength, 0, true);
        tools.addSlider(mUi, "phase "+id, this->id, &(data.phase), 0, 1, 0, true);
        
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        mUi->addSpacer(false);

        tools.addSlider(mUi, "click "+id, this->id, &(data.click), 0.0f, 1.0f, 0, true);
        tools.addSlider(mUi, "out1 "+id, this->id, &(data.out1), 0.0f, 1.0f, 0, true);
        tools.addSlider(mUi, "out2 "+id, this->id, &(data.out2), 0.0f, 1.0f, 0, true);
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        
        mUi->setMinified(false);
    }
    
    
    list<bool> Euclidean::buildRythm(int steps, int beats, float phase){
        phase *= steps;
        list<bool> rythm;
        if(steps == beats){
            for(int i = 0 ; i < steps ; i ++){
                rythm.push_back(true);
            }
            return rythm;
        }
        int pauses = steps - beats;
        bool switcher = false;
        if (beats > pauses) {
            switcher = true;
            // XOR swap pauses and beats
            pauses ^= beats;
            beats ^= pauses;
            pauses ^= beats;
        }
        int per_pulse = floor(pauses / beats);
        int remainder = pauses % beats;
        int noskip = (remainder == 0) ? 0 : floor(beats / remainder);
        int skipXTime = (noskip == 0) ? 0 : floor((beats - remainder)/noskip);
        int count = 0;
        int skipper = 0;
        for (int i = 1; i <= steps; i++) {
            if (count == 0) {
//                rythm[i] = !switcher;
                rythm.push_back(!switcher);
                count = per_pulse;
                if (remainder > 0 && skipper == 0) {
                    count++;
                    remainder--;
                    skipper = (skipXTime > 0) ? noskip : 0;
                    skipXTime--;
                } else {
                    skipper--;
                }
            } else {
                rythm.push_back(switcher);
//                rythm[i] = switcher;
                count--;
            }
        }
        if (switcher) {
            // XOR swap pauses and beats
            pauses ^= beats;
            beats ^= pauses;
            pauses ^= beats;
        }
        for(int i = 0 ; i < phase ; i ++){
            rythm.push_front(rythm.back());
            rythm.pop_back();
        }
        return rythm;
    }
    
    
    
}
