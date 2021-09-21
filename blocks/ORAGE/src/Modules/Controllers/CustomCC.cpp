//
//  NanoKontrol.cpp
//  ORAGE
//
//  Created by Vincent Evrard on 30/09/20.
//

#include "CustomCC.h"



using namespace ogre;
using namespace cinder;
using namespace cinder::gl;
using namespace std;
using namespace reza::ui;

namespace ogre {
    
    int CustomCC::COUNT = 0;

    CustomCC::CustomCC(string name, JsonTree jsonData,  vec2 origin, vec2 size, gl::Context * mMainWinCtx ) :
        ModuleCommon(name+" "+ tools.to_roman(CustomCC::COUNT), origin, size),
        mMainWinCtx(mMainWinCtx)
    {
        if(jsonData.getNumChildren()!=0){
            data = DATA(jsonData);
        }
        this->typeName = "CustomCC";
    }
    
    void CustomCC::setup(){
        ModuleCommon::setup();
        setupUI();
        for( const auto &dev : Serial::getDevices() )
            console() << "Device: " << dev.getName() << endl;

        try {
            Serial::Device dev = Serial::findDeviceByNameContains( "tty.wchusbseria" );
            mSerial = Serial::create( dev, 9600 );
            cout<< "connnect to " << dev.getName() << endl;
        }
        catch( SerialExc &exc ) {
            cout<< "coult not initialize the serial device" << endl;
        }
        
    }
    void CustomCC::onEvent(CallbackType &callback){
        callbacks.push_back(callback);
    }
    
    void CustomCC::trigEvent(int channel, int control, int value){
        auto it = callbacks.begin(), end = callbacks.end();
        while(it != end){
            (*it)(channel, control, value);
            it++;
        }
    }
    /*
    void CustomCC::midiListener(midi::Message msg)
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
     */
    
    void CustomCC::update(){
        if(mMainWinCtx != gl::Context::getCurrent()){
            return;
        }
        ModuleCommon::update();
        if( mSerial != NULL && mSerial->getNumBytesAvailable() > 0 ) {
            uint8_t in = mSerial->readByte();
            uint8_t i;
            for(i=0x80;i!=0;i>>=1)
                printf("%c",(in&i)?'1':'0');
            cout<<endl;
            uint8_t aid = (in >> 5);
            uint8_t pid = (in >> 2) & 0b00000111;
            uint8_t val = in & 0b00000011;
            //cout << aid << " " << pid << " " << val << endl;
            //trigEvent(0, (c & 0xF0) >> 4, (c & 0x0F) - 1);
        }
        if(mSerial != NULL){
                mSerial->flush();
        }
    }
    
    void CustomCC::draw(){
        ModuleCommon::draw();
    }
    
    void CustomCC::setupUI(){
        mUi = nullptr;
    }
}
