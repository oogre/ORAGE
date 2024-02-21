//
//  MidiFighter.hpp
//  ORAGE
//
//  Created by Vincent Evrard on 30/09/20.
//

#ifndef MidiInput_hpp
#define MidiInput_hpp

#include "ModuleBase.h"
#include "MidiHeaders.h"
#include <sys/time.h>
#include <stdio.h>


using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;
/*
 4,  8, 12,  16,  20,  24,  28, 123, 127,
32, 36, 40,  44,  48,  52,  56, 123, 127,
60, 64, 68,  72,  76,  80,  84, 123, 127,
88, 92, 96, 100, 104, 108, 112, 123, 127
*/
namespace ogre {
    class MidiInput : public ModuleCommon{
        
        typedef const std::function<void(int,int,int)> CallbackType;
        vector<CallbackType> callbacks;
        gl::Context * mMainWinCtx;
        
        MidiInput(string name, JsonTree data, vec2 origin, vec2 size, gl::Context * mMainWinCtx);
        
        std::string status;
        double lastTickAt = 0;
        double averageDeltaTick = 0 ;
        static const int ccLEn = 16;
        
        string midiPortName;
        TextInputRef midiName;
        
        void initMidi();
    public:
        
        struct CC {
            float value = 0;
        };
        
        struct DATA {
            CC cc[ccLEn];
            DATA(){}
            DATA(JsonTree data)
            {}
        };
        
        void midiListener(midi::Message msg);
        
        midi::Input mInput;
        
        DATA data;
        
        virtual ~MidiInput(){
            mInput.closePort();
            data.~DATA();
            mMainWinCtx = nullptr;
        }
        static int COUNT;
        
        typedef std::shared_ptr<class MidiInput> MidiInputRef;
        
        static MidiInputRef create( const std::string name, vec2 origin, gl::Context * mMainWinCtx, JsonTree data = JsonTree())
        {
            MidiInput::COUNT++;
            return MidiInputRef( new MidiInput( name, data, origin, vec2(WIDTH*0.8, 280), mMainWinCtx ) );
        }
        
        virtual JsonTree getData() override {
            {
                JsonTree obj = ModuleCommon::getData();
                obj.addChild(JsonTree("type", "MidiInput"));
                obj.addChild(JsonTree("name", this->midiName->getValue()));
                JsonTree sub = JsonTree::makeObject("data");
                for(int i = 0 ; i < ccLEn ; i ++){
                    sub.addChild(JsonTree("cc"+std::to_string(i), data.cc[i].value));
                }
                obj.pushBack(sub);
                return obj;
            }
        };
        void setupUI() override;
        void setup() override;
        void update() override;
        void draw() override;
        
        void onEvent(CallbackType &callback);
        void trigEvent(int channel, int control, int value);
        
    };
    
    typedef std::shared_ptr<class MidiInput> MidiInputRef;
}


#endif /* MidiInput_hpp */
