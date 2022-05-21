//
//  Euclidean.cpp
//  orage
//
//  Created by Vincent Evrard on 2017-04-22.
//
//

#include "Passthrough.h"

using namespace ogre;
using namespace cinder;
using namespace cinder::gl;
using namespace std;
using namespace reza::ui;

namespace ogre {
    
    int Passthrough::COUNT = 0;
    
    
    Passthrough::Passthrough( std::string name, JsonTree jsonData,  vec2 origin, vec2 size, gl::Context * mMainWinCtx ) : ModuleCommon(name+" "+ tools.to_roman(Passthrough::COUNT), origin, size){
        if(jsonData.getNumChildren()!=0){
            data = DATA(jsonData);
        }
        this->mMainWinCtx = mMainWinCtx;
        this->typeName = "Passthrough";
     
    }
    
    void Passthrough::setup(){
        ModuleCommon::setup();
        setupUI();
    }
    
    void Passthrough::update(){
        if(mMainWinCtx != gl::Context::getCurrent()){
            return;
        }
        ModuleCommon::update();
        
        if(data.pass > 0.5 ){
            data.output = data.value;
        }
    }
    
    void Passthrough::setupUI(){
        ModuleCommon::setupUI();
        mUi->setColorBack(ColorAT<float>(vec4(.1f, .2f, .3f, .4f)));
        //mUi->setColorFill(ColorAT<float>(vec4(.8f, .9f, 1.f, .6f)));
        mUi->setColorFillHighlight(ColorAT<float>(vec4(.1f, .9f, 1.f, 1.f)));
        
        string id = toString(COUNT);
        CanvasRef v = Canvas::create("Passthrough"+id, ci::app::getWindow());
        
        
        
        tools.addSlider(mUi, "pass "+id, this->id, &(data.pass), 0, 1, 0, true);
        tools.addSlider(mUi, "value "+id, this->id, &(data.value), 0, 1, 0, true)->followOnHit = false;;
        
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
