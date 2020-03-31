//
//  Structs.hpp
//  ORAGE2
//
//  Created by Vincent Evrard on 2017-05-05.
//
//

#ifndef Structs_hpp
#define Structs_hpp


#include "Config.h"


#include <boost/dynamic_bitset.hpp>



namespace ogre {


class BitsetTools {
    public :
    
    boost::dynamic_bitset<> average(const boost::dynamic_bitset<> * input){
        boost::dynamic_bitset<> output(input->size());
        if(input->any()){
            int sum = 0 ;
            int count = 0 ;
            for(int i = 0 ; i < input->size() ; i ++){
                count += (*input)[i];
                sum += ((*input)[i] ? (i+1) : 0);
            };
            output[(sum/count)-1] = true;
        }
        return output;
    }
    
    float normalize(const boost::dynamic_bitset<> * input){
        if(!input->any()) return 0.0f;
        return (min(input)+1.0f)/(float)input->size();
    }
    
    float min(const boost::dynamic_bitset<> * input){
        if(!input->any()) return 0.0f;
        for(int i = 0 ; i < input->size() ; i ++){
            if((*input)[i]) return (float)(input->size() - i);
        };
        return 0.0f;
    }
    
    float max(const boost::dynamic_bitset<> * input){
        if(!input->any()) return 0.0f;
        for(int i = input->size()-1 ; i >=0  ; i --){
            if((*input)[i]) return (float)(input->size() - i);
        };
        return 0.0f;
    }
    
    boost::dynamic_bitset<> minimum(const boost::dynamic_bitset<> * input){
        boost::dynamic_bitset<> output(input->size());
        if(!input->any()) return output;
        for(int i = 0 ; i < input->size() ; i ++){
            if((*input)[i]){
                output[i] = true;
                return output;
            }
        };
        return output;
    }
    
    boost::dynamic_bitset<> maximum(const boost::dynamic_bitset<> * input){
        boost::dynamic_bitset<> output(input->size());
        if(!input->any()) return output;
        for(int i = input->size()-1 ; i >= 0 ; i --){
            if((*input)[i]){
                output[i] = true;
                return output;
            }
        };
        return output;
    }
    
    int count(const boost::dynamic_bitset<> * input){
        int sum = 0;
        if(input->all()) return input->size();
        if(!input->any()) return 0;
        
        for(int i = input->size()-1 ; i >=0  ; i --){
            if((*input)[i]) sum++;
        };
        return sum;
    }
    
    
    boost::dynamic_bitset<> toOctave(const boost::dynamic_bitset<> * input){
        boost::dynamic_bitset<> output (Config::OCTAVE_LEN);
        for(int i = 0 ; i < input->size() ; i ++){
            output[(i-1)%Config::OCTAVE_LEN] = output[(i-1)%Config::OCTAVE_LEN] | (*input)[i];
        };
        return output;
    }
    
    boost::dynamic_bitset<> enlarge(const boost::dynamic_bitset<> * input){
        boost::dynamic_bitset<> output(Config::NOTE_LEN);
        for(int i = 0 ; i < Config::NOTE_LEN ; i ++){
            output[i] = (*input)[(i-1)%input->size()];
        };
        return output;
    }
    
};


extern BitsetTools bitsetTools;



struct Note;

class Data {
    public :
    static vector<Note> notes;
    static int noteTotal;
    static int noteOnCounter;
    static int noteOffCounter;
    
    static Note * getNote(int pitch);
    static Note * getNoteByNum(int id);
};

extern Data data;


struct Noteset{
    boost::dynamic_bitset<> ID;
    boost::dynamic_bitset<> IDOneOctave;
    vector<Note *>notes;
    Noteset(){
        ID = boost::dynamic_bitset<>(Config::NOTE_LEN);
        IDOneOctave = boost::dynamic_bitset<>(Config::OCTAVE_LEN);
    };
    Noteset(boost::dynamic_bitset<> * ID):Noteset(){
        this->build(ID);
    }
    typedef std::shared_ptr<struct Noteset> NotesetRef;

    static NotesetRef create(boost::dynamic_bitset<> * ID){
        return NotesetRef( new Noteset( ID ) );
    }
    static NotesetRef create(){
        return NotesetRef( new Noteset() );
    }
    int length = 0 ;
    int lengthOneOctave = 0 ;
    float playedAt = 0.f ;
    void addNote(Note * n);
    void removeNote(Note * n);
    void clear();
    void build(boost::dynamic_bitset<> * ID){
        this->clear();
        for(int i = 1 ; i <= ID->size() ; i++){
            if((*ID)[ID->size() - i]){
                this->addNote(Data::getNoteByNum(i-1));
            }
        }
    }
};
typedef std::shared_ptr<struct Noteset> NotesetRef;
    
    
    


struct Note{
    boost::dynamic_bitset<> ID;
    boost::dynamic_bitset<> IDOneOctave;
    
    int num;
    float norm;                 // [0, 1[
    int pitch;                  // [21, 108]
    Config::NOTE_NAME note;     // C, C#, D, D#, E, F, F#, G, G#, A, A#, B
    int octave;                 // [0, 8]
    bool isPlayed = false;
    long playedCount = 0;
    float playedCountMem = 0.0f;
    
    float playedAt = 0.0f;
    float playedTime = 0.0f;
    float playedTimeMem = 0.0f;
    
    //float playedTimeMem = 0.0f;
    Note(int num, boost::dynamic_bitset<> * ID){
        this->ID = * ID;
        this->IDOneOctave = bitsetTools.toOctave(ID);
        this->num = num;
        this->norm = (num+1.0f)/(float)Config::NOTE_LEN;
        this->pitch = num + 21;
        this->note = static_cast<Config::NOTE_NAME>(Config::NOTE_NAME::A + num % Config::OCTAVE_LEN);
        this->octave = (num+9)/Config::OCTAVE_LEN;
    }
    string getName();
    string getNoteName();
    string toString();
    void active(){
        if(isPlayed)return;
        isPlayed = true;
        playedAt = cinder::app::getElapsedSeconds();
        playedCount++;
        playedCountMem++;
    };
    void disactive(){
        if(!isPlayed)return;
        isPlayed = false;
        playedTime += cinder::app::getElapsedSeconds() - playedAt;
        playedTimeMem += cinder::app::getElapsedSeconds() - playedAt;
    };
    void update(){
        playedTimeMem *= Config::NOTE_FORGET_RATE;
        playedCountMem *= Config::NOTE_FORGET_RATE;
    };
    float time(bool memory = false){
        if(isPlayed){
            return cinder::app::getElapsedSeconds() - playedAt + (memory ? playedTimeMem : playedTime);
        }
        else{
            return memory ? playedTimeMem : playedTime;
        }
    }
};

/*
struct Chord{
    Noteset noteset;
    float diffWithLastChord;
    float diffWithLastChordByNote;
    float playedCount;
    float playedCountMem;
    float playedAt;
};*/
    
}

#endif /* Structs_hpp */
