//
//  HandSplitter.cpp
//  ORAGE2
//
//  Created by Vincent Evrard on 2017-05-07.
//
//

#include "HandSplitter.h"



using namespace ogre;
using namespace cinder;
using namespace cinder::gl;
using namespace std;
using namespace reza::ui;

namespace ogre {
    
    int HandSplitter::COUNT = 0;
    
    HandSplitter::HandSplitter( std::string name, vec2 origin, vec2 size, gl::Context * mMainWinCtx ) : ModulePiano(name+" "+ tools.to_roman(HandSplitter::COUNT), origin, size, 1, Config::MAX_HAND){
        this->mMainWinCtx = mMainWinCtx;
        
        for(unsigned int b = 0 ; b < Config::MAX_HAND ; b++){
            hands.push_back(Noteset::create());
            char current = ((char)((int)'A'+b));
            outputs[current] = hands.at(b);
        }
    }
    
    void HandSplitter::setup(){
        ModulePiano::setupUI();
        setupUI();
    }
    
    
    void HandSplitter::update(){
        
        if(mMainWinCtx != gl::Context::getCurrent()){
            return;
        }
        ModulePiano::update();
        
        for(unsigned int b = 0 ; b < Config::MAX_HAND ; b++){
            hands.at(b)->clear();
        }
        
        if(!inputs['A']) return;
        
        int j = 0 ;
        auto it = inputs['A']->notes.begin(), final_it = inputs['A']->notes.end();
        for (; it != final_it; it++){
            hands.at(j)->addNote((*it));
            int dist = final_it != (it+1) ? (*(it+1))->pitch - (*it)->pitch : 0;
            if(final_it != (it+1) && (hands.at(j)->notes.size() >= Config::MAX_NOTE_PER_HAND || dist >= Config::MAX_DIST_PER_HAND) && j < Config::MAX_HAND-1){
                j++;
            }
        }
        
        if(inputs['A']->notes.size()==0){
            countRate = 0;
        }else{
            countRate = (j+1);
        }
        count += (countRate - count)* 0.1f;
    }
    
    void HandSplitter::setupUI(){
        mUi->setColorBack(ColorAT<float>(vec4(.1f, .2f, .3f, .4f)));
        //mUi->setColorFill(ColorAT<float>(vec4(.8f, .9f, 1.f, .6f)));
        mUi->setColorFillHighlight(ColorAT<float>(vec4(.1f, .9f, 1.f, 1.f)));
        
        ModulePiano::setupUI();
        setupInput();
        setupOutput();
        
        tools.addSlider(mUi, "count", this->id, &count, 0, Config::MAX_HAND);
        
        mUi->setMinified(true);
    }
};
