//
//  MidiFighter.cpp
//  ORAGE
//
//  Created by Vincent Evrard on 20/11/23.
//

#include "MidiIn.h"



using namespace ogre;
using namespace cinder;
using namespace cinder::gl;
using namespace std;
using namespace reza::ui;

namespace ogre {
    
    int MidiIn::COUNT = 0;

    MidiIn::MidiIn(string name, JsonTree jsonData,  vec2 origin, vec2 size, gl::Context * mMainWinCtx ) :
        ModuleCommon(name+" "+ tools.to_roman(MidiIn::COUNT), origin, size),
        mMainWinCtx(mMainWinCtx)
    {
        if(jsonData.getNumChildren()!=0){
            data = DATA(jsonData);
        }
        this->typeName = "baveNoire";
    }
    
    void MidiIn::setup(){
        ModuleCommon::setup();
        setupUI();
        mInput.listPorts();
        if (mInput.getNumPorts() > 0)
        {
            int i = 0;
            for (; i < mInput.getNumPorts(); i++){
                cout<<mInput.getPortName(i)<<endl;
                if (mInput.getPortName(i).find(this->typeName) != std::string::npos) {
                    cout<< "connnect to " << mInput.getPortName(i) << endl;
                    mInput.openPort(i);
                    break;
                }
                cout<<mInput.getPortName(i)<<endl;
            }
            if(i < mInput.getNumPorts()){
                mInput.midiSignal.connect([this](midi::Message msg) { midiListener(msg); });
            }
        }
    }
    void MidiIn::onEvent(CallbackType &callback){
        callbacks.push_back(callback);
    }
    
    void MidiIn::trigEvent(int channel, int control, int value){
        auto it = callbacks.begin(), end = callbacks.end();
        while(it != end){
            (*it)(channel, control, value);
            it++;
        }
    }
    
    void MidiIn::midiListener(midi::Message msg)
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
    
    void MidiIn::update(){
        if(mMainWinCtx != gl::Context::getCurrent()){
            return;
        }
        ModuleCommon::update();
    }
    
    void MidiIn::draw(){
        ModuleCommon::draw();
    }
    
    void MidiIn::setupUI(){
        ModuleCommon::setupUI();
        mUi->setColorBack(ColorAT<float>(vec4(.1f, .2f, .3f, .4f)));
        mUi->setColorFillHighlight(ColorAT<float>(vec4(.1f, .9f, 1.f, 1.f)));
        
        for(int i = 0; i < 16 ; i ++){
            tools.addSlider(mUi, "cc "+toString(i), this->id, &(data.cc[i].value), 0, 127, 0);
        }
        
        mUi->autoSizeToFitSubviews();
        
    }
}
