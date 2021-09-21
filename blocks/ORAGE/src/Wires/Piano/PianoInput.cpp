//
//  PianoInput.cpp
//  ORAGE2
//
//  Created by Vincent Evrard on 2017-05-04.
//
//

#include "PianoInput.h"

using namespace ogre;
using namespace cinder;
using namespace cinder::gl;
using namespace std;
using namespace reza::ui;

namespace ogre {
    
    PianoInputRef PianoInput::singleton = nullptr;
    int PianoInput::COUNT = 0;
    
    PianoInput::PianoInput( std::string name, vec2 origin, vec2 size, gl::Context * mMainWinCtx ) : ModulePiano(name+" "+ tools.to_roman(PianoInput::COUNT), origin, size, 0, 1){
        this->mMainWinCtx = mMainWinCtx;
        
        playingNotes = Noteset::create();
        
        boost::dynamic_bitset<> _ID(Config::NOTE_LEN);
        _ID[Config::NOTE_LEN-1]=1;
        for(int i = 0 ; i < Config::NOTE_LEN ; i ++){
            Note n(i, &_ID);
            Data::notes.push_back(n);
            _ID>>=1;
        }
        
        
        outputs['A'] = playingNotes;
        
        try {
            Serial::Device dev = Serial::findDeviceByNameContains( "tty.usbmodem" );
            console() << "Device: " << dev.getName() << endl;
            mSerial = Serial::create( dev, 9600 );
            mSerial->flush();
        }
        catch( SerialExc &exc ) {
            CI_LOG_EXCEPTION( "coult not initialize the serial device", exc );
            exit( -1 );
        }
        
    }
    
    void PianoInput::setup(){
        ModulePiano::setupUI();
        setupUI();
        
    }

    
    void PianoInput::update(){
        
        if(mMainWinCtx != gl::Context::getCurrent()){
            return;
        }
        ModulePiano::update();
        
        /*

        if(!mSerial){
            in = rand() % 2 == 1 ? NOTE_ON : NOTE_OFF;
            in += 1;
            pitch = ((rand() % Config::NOTE_LEN)) + Config::MIN_PITCH;
            if(Note * currentNote = Data::getNote(pitch)){
                if(currentNote->isPlayed){
                    in = NOTE_OFF + 1;
                }
            }
            //in = ci::app::getElapsedFrames() % 3 == 1 ? 0x81 : 0x91;
            //pitch = ((ci::app::getElapsedFrames()/3) % Config::NOTE_LEN) + Config::MIN_PITCH;
            
            velocity = 10;
        }
        else */
        while(mSerial->getNumBytesAvailable()>=3) {
            int pitch = 0, velocity = 0;
            uint8_t in = mSerial->readByte() ^ NOTE_OFF;

            if (in < CHANNEL_LEN || in - CHANNEL_LEN < CHANNEL_LEN){
                pitch = mSerial->readByte();
                velocity = mSerial->readByte();
                if (in < CHANNEL_LEN){
                    if(Note * currentNote = Data::getNote(pitch)){
                        //cout<<"NOTE_OFF : "<<currentNote->getName()<<endl;
                        currentNote->disactive();
                        playingNotes->removeNote(currentNote);
                        Data::noteOffCounter++;
                    }
                }else if (in - CHANNEL_LEN < CHANNEL_LEN){
                    if(Note * currentNote = Data::getNote(pitch)){
                        //cout<<"NOTE_ON : "<<currentNote->getName()<<endl;
                        currentNote->active();
                        playingNotes->addNote(currentNote);
                        Data::noteTotal++;
                        Data::noteOnCounter++;
                    }
                }
            }
        }
        for(int i = 0 ; i < Config::NOTE_LEN ; i ++){
            Data::notes.at(i).update();
        }
    }
    
    void PianoInput::setupUI(){
        mUi->setColorBack(ColorAT<float>(vec4(.1f, .2f, .3f, .4f)));
        //mUi->setColorFill(ColorAT<float>(vec4(.8f, .9f, 1.f, .6f)));
        mUi->setColorFillHighlight(ColorAT<float>(vec4(.1f, .9f, 1.f, 1.f)));
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        
        
        tools.addSlider2(mUi, "NOTE FORGET RATE", this->id, &Config::NOTE_FORGET_RATE, 0.8f, 1.0f);
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        
        
        
        ModulePiano::setupUI();
        setupInput();
        setupOutput();
        mUi->setMinified(false);
    }
};
