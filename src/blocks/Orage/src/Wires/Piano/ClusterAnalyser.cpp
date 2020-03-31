//
//  ClusterAnalyser.cpp
//  PianoProjete
//
//  Created by Vincent Evrard on 2017-06-15.
//
//

#include "ClusterAnalyser.h"


using namespace ogre;
using namespace cinder;
using namespace cinder::gl;
using namespace std;
using namespace reza::ui;

namespace ogre {
    
    int ClusterAnalyser::COUNT = 0;
    
    ClusterAnalyser::ClusterAnalyser( std::string name, vec2 origin, vec2 size, gl::Context * mMainWinCtx ) : ModulePiano(name+" "+ tools.to_roman(ClusterAnalyser::COUNT), origin, size, 1, 0){
        this->mMainWinCtx = mMainWinCtx;
    }
    
    void ClusterAnalyser::setup(){
        ModulePiano::setupUI();
        setupUI();
    }
    
    
    void ClusterAnalyser::update(){
        
        if(mMainWinCtx != gl::Context::getCurrent()){
            return;
        }
        ModulePiano::update();
        for(int i = 0 ; i <= Config::CHORD_NAME::SEPTIEME_MAJEURE ; i++){
            chords[i] -= max(chords[i] * inertie, 0.0f);
        }
        
        if(inputs['A'] && inputs['A']->length>0){
            if(oldInputID != inputs['A']->ID){
                oldInputID = inputs['A']->ID;

                auto it = inputs['A']->notes.begin();
                auto end = inputs['A']->notes.end();
                for(; it != end ; it++){
                    Config::NOTE_NAME base = (*it)->note;
                
                    for(auto it2 = it+1; it2 != end ; it2++){
                        Config::NOTE_NAME current = (*it2)->note;
                        if(base == current)continue;
                        Config::CHORD_NAME name = (Config::CHORD_NAME)(abs(current - base));
                        chords[name]++;
                    }
                }
            }
        }
    }
    
    void ClusterAnalyser::setupUI(){
        mUi->setColorBack(ColorAT<float>(vec4(.1f, .2f, .3f, .4f)));
        //mUi->setColorFill(ColorAT<float>(vec4(.8f, .9f, 1.f, .6f)));
        mUi->setColorFillHighlight(ColorAT<float>(vec4(.1f, .9f, 1.f, 1.f)));
        
        ModulePiano::setupUI();
        setupInput();
        setupOutput();
        
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        tools.addSlider(mUi, "Inertie", this->id, &inertie, 0, 0.3);
        
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        for(int i = 0 ; i <= Config::CHORD_NAME::SEPTIEME_MAJEURE ; i++){
            chordsRate[i] = 0;
            chords[i] = 0;
            tools.addSlider(mUi, Config::CHORD_NAME_TO_STRING[i], this->id, chords+i, 0, 10);
        }
        
        mUi->setMinified(false);
    }
};
