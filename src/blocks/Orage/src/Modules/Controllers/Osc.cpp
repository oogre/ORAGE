//
//  Osc.cpp
//  ORAGE
//
//  Created by Vincent Evrard on 3/04/20.
//

#include "Osc.h"

using namespace ogre;
using namespace cinder;
using namespace cinder::gl;
using namespace std;
using namespace osc;
using namespace reza::ui;

namespace ogre {
    
    int Osc::COUNT = 0;
    map<int, Listener> Osc::LISTENERS = {};
    
    Osc::Osc(string name, JsonTree jsonData,  vec2 origin, vec2 size, gl::Context * mMainWinCtx )
        : ModuleCommon(name+" "+ tools.to_roman(Osc::COUNT), origin, size),
          mMainWinCtx(mMainWinCtx)
    {
        if(jsonData.getNumChildren()!=0){
            data = DATA(jsonData);
        }
        this->typeName = "Osc";
    }
    
    void Osc::setup(){
        ModuleCommon::setup();
        setupUI();
    }
    
    void Osc::update(){
        if(mMainWinCtx != gl::Context::getCurrent()){
            return;
        }
        
        ModuleCommon::update();
        if(!data.active){
            data.address = address->getValue();
            try
            {
                data.port = stoi(port->getValue());
            }
            catch (invalid_argument const &e)
            {}
            catch (out_of_range const &e)
            {}
            if(!port->isEnabled()){
                port->setEnabled(true);
            }
        }
        
        
        if(data.active && LISTENERS.find(data.port) == LISTENERS.end()){
            LISTENERS[data.port] = Listener();
            LISTENERS[data.port].setup( data.port );
            cout << "." << endl;
        }
        
        if(!data.active || !LISTENERS[data.port].hasWaitingMessages())return;
        
        
        
        osc::Message message;
        LISTENERS[data.port].getNextMessage( &message );
        
        if(message.getAddress().compare(data.address) == 0){
            for (int i = 0; i < message.getNumArgs(); i++) {
                if (message.getArgType(i) == osc::TYPE_FLOAT){
                    data.value = message.getArgAsFloat(i);
                }
            }
        }
    }
    
    void Osc::setupUI(){
        ModuleCommon::setupUI();
        mUi->setColorBack(ColorAT<float>(vec4(.1f, .2f, .3f, .4f)));
        mUi->setColorFillHighlight(ColorAT<float>(vec4(.1f, .9f, 1.f, 1.f)));
        
        string id = toString(COUNT);
        CanvasRef v = Canvas::create("Osc"+id, ci::app::getWindow());
        ToggleRef on = Toggle::create("ON "+id, &(data.active), Button::Format().label(true));
        
        mUi->addSubViewDown(on);
        mUi->addSubViewToHeader(on);
        address = mUi->addTextInput(data.address);
        port = mUi->addTextInput(to_string(data.port));
        
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        tools.addSlider(mUi, "value "+id, this->id, &(data.value), 0, 1, 0, true);
        mUi->setMinified(false);
    }
}
