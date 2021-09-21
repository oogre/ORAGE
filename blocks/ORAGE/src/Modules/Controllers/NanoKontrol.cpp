//
//  NanoKontrol.cpp
//  ORAGE
//
//  Created by Vincent Evrard on 30/09/20.
//

#include "NanoKontrol.h"



using namespace ogre;
using namespace cinder;
using namespace cinder::gl;
using namespace std;
using namespace reza::ui;

namespace ogre {
    
    int NanoKontrol::COUNT = 0;

    NanoKontrol::NanoKontrol(string name, JsonTree jsonData,  vec2 origin, vec2 size, gl::Context * mMainWinCtx ) :
        ModuleCommon(name+" "+ tools.to_roman(NanoKontrol::COUNT), origin, size),
        mMainWinCtx(mMainWinCtx)
    {
        if(jsonData.getNumChildren()!=0){
            data = DATA(jsonData);
        }
        this->typeName = "nanoKONTROL";
    }
    
    void NanoKontrol::setup(){
        ModuleCommon::setup();
        setupUI();
        mInput.listPorts();
        if (mInput.getNumPorts() > 0)
        {
            int i = 0;
            for (; i < mInput.getNumPorts(); i++){
                if (mInput.getPortName(i).find(this->typeName) != std::string::npos) {
                    cout<< "connnect to " << mInput.getPortName(i) << endl;
                    mInput.openPort(i);
                    break;
                }
            }
            if(i < mInput.getNumPorts()){
                mInput.midiSignal.connect([this](midi::Message msg) { midiListener(msg); });
            }
        }
    }
    void NanoKontrol::onEvent(CallbackType &callback){
        callbacks.push_back(callback);
    }
    
    void NanoKontrol::trigEvent(int channel, int control, int value){
        auto it = callbacks.begin(), end = callbacks.end();
        while(it != end){
            (*it)(channel, control, value);
            it++;
        }
    }
    
    void NanoKontrol::midiListener(midi::Message msg)
    {
        // This will be called on on the main thread and
        // safe to use with update and draw.
        switch (msg.status)
        {
            case MIDI_CONTROL_CHANGE:
                trigEvent(msg.channel, msg.control, msg.value);
                break;
            default:
                break;
        }
    }
    
    void NanoKontrol::update(){
        if(mMainWinCtx != gl::Context::getCurrent()){
            return;
        }
        ModuleCommon::update();
    }
    
    void NanoKontrol::draw(){
        ModuleCommon::draw();
    }
    
    void NanoKontrol::setupUI(){
        mUi = nullptr;
    }
}
