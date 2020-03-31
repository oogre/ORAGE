//
//  ModulePiano.cpp
//  ORAGE2
//
//  Created by Vincent Evrard on 2017-05-05.
//
//

#include "ModulePiano.h"


namespace ogre {


ModulePiano::ModulePiano(const std::string name, vec2 origin, vec2 size, const int & nInput , const int & nOutput ):
    ModuleBase<NotesetRef>(name, origin, size, nInput, nOutput){
}

void ModulePiano::setupInput(){
    ModuleBase<NotesetRef>::setupInput();
    Wires * _w = &wires;
    auto it = inputs.begin(), end = inputs.end();
    auto bt = inputBtns.begin();
    string moduleName = this->name;
    int moduleId = this->id;
    for( ; it != end ; it ++, bt++){
        auto * noteset = &(it->second);
        auto b = (*bt);
        b->setCallback(
                       [b, _w, noteset, moduleName, moduleId](bool a) {
                           if(a){
                               _w->clickOnPianoLinker(moduleName, noteset, b, true, moduleId);
                           }
                       });
    }
    
}
void ModulePiano::setupOutput(){
    ModuleBase<NotesetRef>::setupOutput();
    Wires * _w = &wires;
    auto it = outputs.begin(), end = outputs.end();
    auto bt = outputBtns.begin();
    string moduleName = this->name;
    int moduleId = this->id;
    for( ; it != end ; it ++, bt++){
        auto * noteset = &(it->second);
        auto b = (*bt);
        b->setCallback(
                       [b, _w, noteset, moduleName, moduleId](bool a) {
                           if(a){
                               _w->clickOnPianoLinker(moduleName, noteset, b, false, moduleId);
                           }
                       });
    }
}
    
    void ModulePiano::setupUI(){
        ModuleBase<NotesetRef>::setupUI();
    }
    void ModulePiano::setup(){
        ModuleBase<NotesetRef>::setup();
    };
    void ModulePiano::update(){
        ModuleBase<NotesetRef>::update();
    };

    
};
