//
//  Lfos.cpp
//  orage
//
//  Created by Vincent Evrard on 2017-04-22.
//
//

#include "CDelay.h"

using namespace ogre;
using namespace cinder;
using namespace cinder::gl;
using namespace std;
using namespace reza::ui;

namespace ogre {
    
    int CDelay::COUNT = 0;
    
    
    CDelay::CDelay( std::string name, JsonTree jsonData,  vec2 origin, vec2 size, gl::Context * mMainWinCtx ) : ModuleCommon(name+" "+ tools.to_roman(CDelay::COUNT), origin, size){
        if(jsonData.getNumChildren()!=0){
            data = DATA(jsonData);
        }
        this->mMainWinCtx = mMainWinCtx;
        this->typeName = "CDelay";
        for(int i = 0 ; i < CDelay::maxMemoryLength ; i++){
            memory[WCursor] = 0.0;
        }
    }
    
    void CDelay::setup(){
        ModuleCommon::setup();
        setupUI();
        
    }
    
    void CDelay::update(){
        if(mMainWinCtx != gl::Context::getCurrent()){
            return;
        }
        ModuleCommon::update();

        memory[WCursor] = data.value;
        RCursor = WCursor + CDelay::maxMemoryLength - floor(data.length);
        RCursor %= CDelay::maxMemoryLength;
        data.output = memory[RCursor];
        
        WCursor ++;
        WCursor %= CDelay::maxMemoryLength;
        
        
        
        //CDelay::maxMemoryLength
        
        
    }
    
    void CDelay::setupUI(){
        ModuleCommon::setupUI();
        mUi->setColorBack(ColorAT<float>(vec4(.1f, .2f, .3f, .4f)));
        //mUi->setColorFill(ColorAT<float>(vec4(.8f, .9f, 1.f, .6f)));
        mUi->setColorFillHighlight(ColorAT<float>(vec4(.1f, .9f, 1.f, 1.f)));
        
        string id = toString(COUNT);
        CanvasRef v = Canvas::create("CDelay"+id, ci::app::getWindow());
        
        
        tools.addSlider(mUi, "value "+id, this->id, &(data.value), 0, 1, 0, true)->followOnHit = false;
        
        tools.addSlider(mUi, "length "+id, this->id, &(data.length), 0.f, (float)maxMemoryLength-1, 0, true);
        
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        mUi->addSpacer(false);

        tools.addSlider(mUi, "output "+id, this->id, &(data.output), 0.0f, 1.0f, 0, true);
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        
        mUi->setMinified(false);
    }
    
    
    
    
}
