//
//  ProcessCV.cpp
//  ORAGE2
//
//  Created by Vincent Evrard on 2017-06-30.
//
//

#include "ProcessCV.h"


namespace ogre {
    
    int ProcessCV::COUNT = 0;
    ProcessCV::ProcessCV( std::string name, JsonTree jsonData, vec2 origin, vec2 size, gl::Context * mMainWinCtx ) : ModuleCommon(name+" "+ tools.to_roman(ProcessCV::COUNT), origin, size){
        if(jsonData.getNumChildren()!=0){
            data = DATA(jsonData);
        }
        this->mMainWinCtx = mMainWinCtx;
    }
    
    void ProcessCV::setup(){
        ModuleCommon::setup();
        setupUI();
        
    }
    
    void ProcessCV::update(){
        if(mMainWinCtx != gl::Context::getCurrent()){
            return;
        }
        ModuleCommon::update();
        
        data.reverse = 1 - data.master;
    }
    
    void ProcessCV::setupUI(){
        ModuleCommon::setupUI();
        mUi->setColorBack(ColorAT<float>(vec4(.1f, .2f, .3f, .4f)));
        //mUi->setColorFill(ColorAT<float>(vec4(.8f, .9f, 1.f, .6f)));
        mUi->setColorFillHighlight(ColorAT<float>(vec4(.1f, .9f, 1.f, 1.f)));
        
        string id = toString(COUNT);
        CanvasRef v = Canvas::create("LFO"+id, ci::app::getWindow());
        
        mUi->addSpacer(false);
        mUi->addSpacer(false);

        tools.addSlider(mUi, "master "+id, this->id, &(data.master), 0, 1, 0, true);
        tools.addSlider(mUi, "reverse "+id, this->id, &(data.reverse), 0, 1, 0, true);
        
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        
        
        mUi->setMinified(false);
    }
    
    
    
    
}
