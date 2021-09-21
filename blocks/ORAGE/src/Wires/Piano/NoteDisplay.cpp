//
//  NoteDisplay.cpp
//  ORAGE2
//
//  Created by Vincent Evrard on 2017-05-07.
//
//

#include "NoteDisplay.h"


using namespace ogre;
using namespace cinder;
using namespace cinder::gl;
using namespace std;
using namespace reza::ui;

namespace ogre {
    int NoteDisplay::COUNT = 0;
    
    NoteDisplay::NoteDisplay( std::string name, vec2 origin, vec2 size, gl::Context * mMainWinCtx, const int & S ) : ModulePiano(name+" "+ tools.to_roman(NoteDisplay::COUNT), origin, size, 1, 1){
        this->mMainWinCtx = mMainWinCtx;
        this->size = S;
        
        notes.assign(S, 0.f);
        notesRate.assign(S, 0.f);
        
        output = Noteset::create();
        outputs['A'] = output;
    }
    
    void NoteDisplay::setup(){
        ModulePiano::setupUI();
        setupUI();
    }
    
    void NoteDisplay::update(){
        
        if(mMainWinCtx != gl::Context::getCurrent()){
            return;
        }
        ModulePiano::update();
        
        for(int i = 0; i < size ; i ++){
            notes.at(i) -= notes.at(i) * 0.1f;
        }
        
        if(!inputs['A']) return;
        
        if(size == Config::NOTE_LEN){
            output->build(&(inputs['A']->ID));
        } else if (size == Config::OCTAVE_LEN){
            output->build(&(inputs['A']->IDOneOctave));
        }
        
        auto it = output->notes.begin(), final_it = output->notes.end();
        for (; it != final_it; it++){
            notes.at((*it)->num) = 1.0f;
        }
    }
    
    void NoteDisplay::setupUI(){
        mUi->setColorBack(ColorAT<float>(vec4(.1f, .2f, .3f, .4f)));
        //mUi->setColorFill(ColorAT<float>(vec4(.8f, .9f, 1.f, .6f)));
        mUi->setColorFillHighlight(ColorAT<float>(vec4(.1f, .9f, 1.f, 1.f)));
        
        ModulePiano::setupUI();
        setupInput();
        setupOutput();
        
        for(int i = 0; i < size ; i ++){
            tools.addSlider2(mUi, Data::notes.at(i).getName(), this->id, &(notes.at(i)), 0.f, 1.f);
        }
        mUi->setMinified(true);
    }
    
    
    
};
