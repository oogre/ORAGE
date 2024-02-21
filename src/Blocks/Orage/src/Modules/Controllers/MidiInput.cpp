//
//  MidiFighter.cpp
//  ORAGE
//
//  Created by Vincent Evrard on 30/09/20.
//

#include "MidiInput.h"
#include "UI.h"
#include "Triggers.h"



using namespace ogre;
using namespace cinder;
using namespace cinder::gl;
using namespace std;
using namespace reza::ui;

namespace ogre {
    
    int MidiInput::COUNT = 0;

    MidiInput::MidiInput(string name, JsonTree jsonData,  vec2 origin, vec2 size, gl::Context * mMainWinCtx ) :
        ModuleCommon(name+" "+ tools.to_roman(MidiInput::COUNT), origin, size),
        mMainWinCtx(mMainWinCtx)
    {
        midiPortName = name;
        if(jsonData.getNumChildren()!=0){
            data = DATA(jsonData);
        }
    }
    
    void MidiInput::setup(){
        ModuleCommon::setup();
        setupUI();
    }
//    void MidiFighter::onEvent(CallbackType &callback){
//        callbacks.push_back(callback);
//    }
//
//    void MidiFighter::trigEvent(int channel, int control, int value){
//        auto it = callbacks.begin(), end = callbacks.end();
//        while(it != end){
//            (*it)(channel, control, value);
//            it++;
//        }
//    }
    
    void MidiInput::midiListener(midi::Message msg)
    {
        // This will be called on on the main thread and
        // safe to use with update and draw.
        switch (msg.status)
        {
            case MIDI_CONTROL_CHANGE:
                 data.cc[msg.control].value = msg.value;
                break;
            default:
                break;
        }
    }
    
    void MidiInput::update(){
        if(mMainWinCtx != gl::Context::getCurrent()){
            return;
        }
        ModuleCommon::update();
    }
    
    void MidiInput::draw(){
        ModuleCommon::draw();
    }
    
    void MidiInput::setupUI(){
        ModuleCommon::setupUI();
        mUi->setColorBack(ColorAT<float>(vec4(.1f, .2f, .3f, .4f)));
        mUi->setColorFillHighlight(ColorAT<float>(vec4(.1f, .9f, 1.f, 1.f)));

        midiName = mUi->addTextInput(midiPortName, TextInput::Format().trigger(Trigger::END));
        midiName->setCallback([this](string value){
            this->initMidi();
        });
        this->initMidi();
        
        for(int i = 0; i < ccLEn ; i ++){
            tools.addSlider(mUi, "cc "+toString(i), this->id, &(data.cc[i].value), 0, 127, 0);
            if(i > 0 && i % 3 == 0 ){
                mUi->addSpacer(false);
                mUi->addSpacer(false);
                mUi->addSpacer(false);
            }
        }
        
        mUi->autoSizeToFitSubviews();
    }
    
    void MidiInput::initMidi(){
        mInput.listPorts();
        if (mInput.getNumPorts() > 0)
        {
            int i = 0;
            for (; i < mInput.getNumPorts(); i++){
                mInput.closePort();
                if (mInput.getPortName(i).find(this->midiName->getValue()) != std::string::npos) {
                    cout<< "connnect to " << mInput.getPortName(i) << endl;
                    mInput.openPort(i);
                    mInput.midiSignal.connect([this](midi::Message msg) { midiListener(msg); });
                    this->setName(this->midiName->getValue());
                    return;
                }
            }
        }
        this->setName("MIDI-IN");
    }
}
