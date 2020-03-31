//
//  Structs.cpp
//  ORAGE2
//
//  Created by Vincent Evrard on 2017-05-05.
//
//

#include "Structs.h"
#include "cinder/app/App.h"

namespace ogre {
vector<Note> Data::notes;

int Data::noteTotal = 0 ;
int Data::noteOnCounter = 0 ;
int Data::noteOffCounter = 0 ;

Data data;


//--------------------------------------------------------------
Note * Data::getNote(int pitch){
    int num = (pitch)-21;
    return getNoteByNum(num);
};

//--------------------------------------------------------------
Note * Data::getNoteByNum(int num){
    if(num >= 0 && num < Data::notes.size()){
        return &Data::notes.at(num);
    }
    return nullptr;
};


//--------------------------------------------------------------
//--------------------------------------------------------------
// Note function Definition


string Note::getNoteName(){
    return Config::NOTE_NAME_TO_STRING[note];
};

//--------------------------------------------------------------
string Note::getName(){
    return getNoteName() + to_string(octave);
};

//--------------------------------------------------------------
string Note::toString(){
    return          to_string(pitch)
    + " " + Config::NOTE_NAME_TO_STRING[note]
    + " " + to_string(octave)
    + " " + to_string(playedCount)
    + " " + to_string(norm);
};

//--------------------------------------------------------------
//--------------------------------------------------------------
// Noteset function Definition
void Noteset::addNote(Note * n){
    if(!ID.size()){
        this->ID = boost::dynamic_bitset<>(Config::NOTE_LEN);
    }
    if(!IDOneOctave.size()){
        IDOneOctave = boost::dynamic_bitset<>(Config::OCTAVE_LEN);
    }
    this->ID |= n->ID;
    this->IDOneOctave |= n->IDOneOctave;
    this->length ++;
    this->lengthOneOctave = bitsetTools.count(&IDOneOctave);
    auto it = this->notes.begin(), end = this->notes.end();
    bool flagInserted = false;
    for( ; it != end ; it++){
        if(!(n->pitch>(*it)->pitch)){
            this->notes.insert(it, n);
            flagInserted = true;
            break;
        }
    }
    if(!flagInserted){
        this->notes.push_back(n);
    }
    this->playedAt = cinder::app::getElapsedSeconds();
    
}

//--------------------------------------------------------------
void Noteset::removeNote(Note * n){
    this->ID ^= n->ID;
    this->IDOneOctave = bitsetTools.toOctave(&(this->ID));
    this->length --;
    this->lengthOneOctave = bitsetTools.count(&IDOneOctave);
    for(auto it = this->notes.begin() ; it != this->notes.end() ; it++){
        if((*it)->ID == n->ID){
            this->notes.erase(it);
            break;
        }
    }
    this->playedAt = cinder::app::getElapsedSeconds();
}
void Noteset::clear(){
    this->ID.clear();
    this->IDOneOctave.clear();
    this->notes.clear();
    this->length=0;
    this->lengthOneOctave=0;
    this->playedAt = 0;
}
    
    BitsetTools bitsetTools;
    
}