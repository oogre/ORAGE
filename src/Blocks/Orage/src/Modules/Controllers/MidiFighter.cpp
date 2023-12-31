//
//  MidiFighter.cpp
//  ORAGE
//
//  Created by Vincent Evrard on 30/09/20.
//

#include "MidiFighter.h"



using namespace ogre;
using namespace cinder;
using namespace cinder::gl;
using namespace std;
using namespace reza::ui;

namespace ogre {
    
    int MidiFighter::COUNT = 0;

    MidiFighter::MidiFighter(string name, JsonTree jsonData,  vec2 origin, vec2 size, gl::Context * mMainWinCtx ) :
        ModuleCommon(name+" "+ tools.to_roman(MidiFighter::COUNT), origin, size),
        mMainWinCtx(mMainWinCtx)
    {
        if(jsonData.getNumChildren()!=0){
            data = DATA(jsonData);
        }
        this->typeName = "cyclone";
    }
    
    void MidiFighter::setup(){
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
    void MidiFighter::onEvent(CallbackType &callback){
        callbacks.push_back(callback);
    }
    
    void MidiFighter::trigEvent(int channel, int control, int value){
        auto it = callbacks.begin(), end = callbacks.end();
        while(it != end){
            (*it)(channel, control, value);
            it++;
        }
    }
    
    void MidiFighter::midiListener(midi::Message msg)
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
    
    void MidiFighter::update(){
        if(mMainWinCtx != gl::Context::getCurrent()){
            return;
        }
        ModuleCommon::update();
    }
    
    void MidiFighter::draw(){
        ModuleCommon::draw();
    }
    
    void MidiFighter::setupUI(){
        ModuleCommon::setupUI();
        mUi->setColorBack(ColorAT<float>(vec4(.1f, .2f, .3f, .4f)));
        mUi->setColorFillHighlight(ColorAT<float>(vec4(.1f, .9f, 1.f, 1.f)));
        
        for(int i = 0; i < 16 ; i ++){
            tools.addSlider(mUi, "cc "+toString(i), this->id, &(data.cc[i].value), 0, 127, 0);
        }
        
        mUi->autoSizeToFitSubviews();
        
    }
}
