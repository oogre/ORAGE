//
//  BeatStepPro.cpp
//  ORAGE
//
//  Created by Vincent Evrard on 30/09/20.
//

#include "BeatStepPro.h"



using namespace ogre;
using namespace cinder;
using namespace cinder::gl;
using namespace std;
using namespace reza::ui;

namespace ogre {
    
    int BeatStepPro::COUNT = 0;

    
    int BeatStepPro::TICK_PER_QUARTER_NOTE = 24;
    int BeatStepPro::S_PER_MINUTE = 60;
    float BeatStepPro::S_PER_QUARTER_NOTE = BeatStepPro::S_PER_MINUTE / (float) BeatStepPro::TICK_PER_QUARTER_NOTE;

    BeatStepPro::BeatStepPro(string name, JsonTree jsonData,  vec2 origin, vec2 size, gl::Context * mMainWinCtx )
    : ModuleCommon(name+" "+ tools.to_roman(BeatStepPro::COUNT), origin, size),
    mMainWinCtx(mMainWinCtx)
    {
        if(jsonData.getNumChildren()!=0){
            data = DATA(jsonData);
        }
        this->typeName = "Arturia BeatStep Pro Arturia BeatStepPro";
        ControlUI = SuperCanvas::create("Control : "+name);
        ControlUI->setSize(size + vec2(0, size.y));
        ControlUI->setOrigin(vec2(origin.x, origin.y + mUi->getSize().y));
        
        Sequencer1UI = SuperCanvas::create("Sequencer1 : "+name);
        Sequencer1UI->setSize(size);
        Sequencer1UI->setOrigin(vec2(origin.x + 2 * size.x, origin.y));
        
        Sequencer2UI = SuperCanvas::create("Sequencer2 : "+name);
        Sequencer2UI->setSize(size);
        Sequencer2UI->setOrigin(vec2(origin.x + 3 * size.x, origin.y));
        
        DrumUI = SuperCanvas::create("Drum : "+name);
        DrumUI->setSize(size + vec2(0, size.y));
        DrumUI->setOrigin(vec2(origin.x + 4 * size.x, origin.y));
        
        
    }
    
    void BeatStepPro::setup(){
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
    
    void BeatStepPro::midiListener(midi::Message msg)
    {
        // This will be called on on the main thread and
        // safe to use with update and draw.
        
        switch (msg.status)
        {
            case MIDI_TIME_CLOCK:
            {
                double now = getElapsedSeconds();
                double deltaTick = (now - lastTickAt);
                lastTickAt = now;
                averageDeltaTick += (deltaTick - averageDeltaTick) * 0.01;
                data.BPM = round(S_PER_QUARTER_NOTE / averageDeltaTick);
            }
                break;
            case MIDI_NOTE_ON:
            case MIDI_NOTE_OFF:
                if(msg.channel == 1 || msg.channel == 2){
                    data.sequencer[msg.channel-1].gate = msg.status == MIDI_NOTE_ON ? 1.f : 0.0f;
                    data.sequencer[msg.channel-1].key = msg.pitch % 12;
                    data.sequencer[msg.channel-1].octave = floor(msg.pitch / 12);
                    data.sequencer[msg.channel-1].velocity = msg.velocity;
                }else{
                    if(msg.channel == 10){
                        data.drum[msg.pitch - 36].gate = msg.status == MIDI_NOTE_ON ? 1.f : 0.0f;
                    }
                }
                break;
            case MIDI_CONTROL_CHANGE:
                if(msg.control < 16 && msg.control >= 0){
                    data.cc[msg.control].value = msg.value;
                }
              //  status = "Control: " + toString(msg.control) + "\n" +"Value: " + toString(msg.value);
                break;
            default:
                break;
        }
        
    }
    
    void BeatStepPro::update(){
        if(mMainWinCtx != gl::Context::getCurrent()){
            return;
        }
        ModuleCommon::update();
        ControlUI->update();
        Sequencer1UI->update();
        Sequencer2UI->update();
        DrumUI->update();
    }
    void BeatStepPro::draw(){
        ModuleCommon::draw();
        ControlUI->draw();
        Sequencer1UI->draw();
        Sequencer2UI->draw();
        DrumUI->draw();
    }
    
    void BeatStepPro::setupUI(){
        ModuleCommon::setupUI();
        mUi->setColorBack(ColorAT<float>(vec4(.1f, .2f, .3f, .4f)));
        mUi->setColorFillHighlight(ColorAT<float>(vec4(.1f, .9f, 1.f, 1.f)));
        
        
        
        //CanvasRef v = Canvas::create("BeatStepPro"+toString(COUNT), ci::app::getWindow());
        
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        mUi->addLabel("BPM");
        tools.addSlider(mUi, "BPM", this->id, &(data.BPM), 30, 300, 0, true);
        
        int i = 0;
        for(; i < 4 ; i ++){
            tools.addSlider(ControlUI, "cc "+toString(i), this->id, &(data.cc[i].value), 0, 127, 0);
        }
        ControlUI->addSpacer(false);
        ControlUI->addSpacer(false);
        for(; i < 8 ; i ++){
            tools.addSlider(ControlUI, "cc "+toString(i), this->id, &(data.cc[i].value), 0, 127, 0);
        }
        ControlUI->addSpacer(false);
        ControlUI->addSpacer(false);
        ControlUI->addSpacer(false);
        for(; i < 12 ; i ++){
            tools.addSlider(ControlUI, "cc "+toString(i), this->id, &(data.cc[i].value), 0, 127, 0);
        }
        ControlUI->addSpacer(false);
        ControlUI->addSpacer(false);
        for(; i < 16 ; i ++){
            tools.addSlider(ControlUI, "cc "+toString(i), this->id, &(data.cc[i].value), 0, 127, 0);
        }
        
        tools.addSlider(Sequencer1UI, "octave", this->id, &(data.sequencer[0].octave), 0, 11, 0, true, true)->setColorBack(GREEN);
        tools.addSlider(Sequencer1UI, "key", this->id, &(data.sequencer[0].key), 0, 12, 0, true, true)->setColorBack(GREEN);
        tools.addSlider(Sequencer1UI, "gate", this->id, &(data.sequencer[0].gate), 0, 1, 0, true, true)->setColorBack(GREEN);
        tools.addSlider(Sequencer1UI, "velocity", this->id, &(data.sequencer[0].velocity), 0, 127, 0, true)->setColorBack(GREEN);
        
        
        
        tools.addSlider(Sequencer2UI, "octave", this->id, &(data.sequencer[1].octave), 0, 11, 0, true, true)->setColorBack(YELLOW);
        tools.addSlider(Sequencer2UI, "key", this->id, &(data.sequencer[1].key), 0, 12, 0, true, true)->setColorBack(YELLOW);
        tools.addSlider(Sequencer2UI, "gate", this->id, &(data.sequencer[1].gate), 0, 1, 0, true, true)->setColorBack(YELLOW);
        tools.addSlider(Sequencer2UI, "velocity", this->id, &(data.sequencer[1].velocity), 0, 127, 0, true)->setColorBack(YELLOW);
        
        i = 0;
        
        for(; i < 8 ; i ++){
            tools.addSlider(DrumUI, "gate "+toString(i), this->id, &(data.drum[i].gate), 0, 1, 0, true)->setColorBack(PURPLE);
        }
        DrumUI->addSpacer(false);
        DrumUI->addSpacer(false);
        
        for( ; i < 16 ; i ++){
            tools.addSlider(DrumUI, "gate "+toString(i), this->id, &(data.drum[i].gate), 0, 1, 0, true)->setColorBack(PURPLE);
        }
        
        mUi->setMinified(false);
        
        
        mUi->autoSizeToFitSubviews();
        ControlUI->autoSizeToFitSubviews();
        Sequencer1UI->autoSizeToFitSubviews();
        Sequencer2UI->autoSizeToFitSubviews();
        DrumUI->autoSizeToFitSubviews();
        
        vec2 origin = mUi->getOrigin();
        ControlUI->setOrigin(vec2(origin.x, origin.y + mUi->getSize().y));
        Sequencer1UI->setOrigin(vec2(origin.x + mUi->getSize().x, origin.y));
        Sequencer2UI->setOrigin(vec2(origin.x + mUi->getSize().x, origin.y+Sequencer1UI->getSize().y));
        DrumUI->setOrigin(vec2(origin.x + mUi->getSize().x + Sequencer1UI->getSize().x, origin.y));
    }
}
