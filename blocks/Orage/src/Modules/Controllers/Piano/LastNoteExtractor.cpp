//
//  LastNoteExtractor.cpp
//  PianoProjete
//
//  Created by Vincent Evrard on 2017-05-30.
//
//

#include "LastNoteExtractor.h"



using namespace ogre;
using namespace cinder;
using namespace cinder::gl;
using namespace std;
using namespace reza::ui;

namespace ogre {
    
    int LastNoteExtractor::COUNT = 0;
    
    LastNoteExtractor::LastNoteExtractor( std::string name, vec2 origin, vec2 size, gl::Context * mMainWinCtx ) : ModulePiano(name+" "+ tools.to_roman(LastNoteExtractor::COUNT), origin, size, 1, 2){
        this->mMainWinCtx = mMainWinCtx;
        
        for(unsigned int b = 0 ; b < outputLen ; b++){
            results.push_back(Noteset::create());
            char current = ((char)((int)'A'+b));
            outputs[current] = results.at(b);
        }
    }
    
    void LastNoteExtractor::setup(){
        ModulePiano::setupUI();
        setupUI();
    }
    
    
    void LastNoteExtractor::update(){
        
        if(mMainWinCtx != gl::Context::getCurrent()){
            return;
        }
        ModulePiano::update();
        
        if(!inputs['A']) return;
        
        if(oldInputID != inputs['A']->ID){
            
            if(inputs['A']->length!=0){
                for(unsigned int b = 0 ; b < outputLen ; b++){
                    char current = ((char)((int)'A'+b));
                    outputs[current]->clear();
                }
            
                oldInputID = inputs['A']->ID;
                *outputs['B'] = *inputs['A'];
                Note * tmpLastNotePLayed = nullptr;
                auto it = inputs['A']->notes.begin();
                auto end = inputs['A']->notes.end();
                for(; it != end ; it++){
                    if(tmpLastNotePLayed == nullptr || tmpLastNotePLayed->playedAt < (*it)->playedAt){
                        tmpLastNotePLayed = *it;
                    }
                }
                if(tmpLastNotePLayed){
                    outputs['A']->addNote(tmpLastNotePLayed);
                    outputs['B']->removeNote(tmpLastNotePLayed);
                }
            }
        }
    }
    
    void LastNoteExtractor::setupUI(){
        mUi->setColorBack(ColorAT<float>(vec4(.1f, .2f, .3f, .4f)));
        //mUi->setColorFill(ColorAT<float>(vec4(.8f, .9f, 1.f, .6f)));
        mUi->setColorFillHighlight(ColorAT<float>(vec4(.1f, .9f, 1.f, 1.f)));
        
        ModulePiano::setupUI();
        setupInput();
        setupOutput();
        
        mUi->setMinified(true);
    }
};