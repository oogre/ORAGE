//
//  BeatStepPro.hpp
//  ORAGE
//
//  Created by Vincent Evrard on 30/09/20.
//

#ifndef BeatStepPro_hpp
#define BeatStepPro_hpp

#include "ModuleBase.h"
#include "../blocks/MIDI2/src/MidiHeaders.h"
#include <sys/time.h>
#include <stdio.h>

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;


namespace ogre {
    class BeatStepPro : public ModuleCommon{
        
        gl::Context * mMainWinCtx;
        
        BeatStepPro(string name, JsonTree data, vec2 origin, vec2 size, gl::Context * mMainWinCtx);
        
        std::string status;
        double lastTickAt = 0;
        double averageDeltaTick = 0 ;
        static const int ccLEn = 16;
        
        ColorAT<float> GREEN = ColorAT<float>(vec4(0.0f, 1.0f, 0.3f, .4f));
        ColorAT<float> YELLOW = ColorAT<float>(vec4(1.0f,0.7f, 0.0f, .6f));
        ColorAT<float> PURPLE = ColorAT<float>(vec4(1.0f,0.0f, 0.7f, .6f));
        
        SuperCanvasRef ControlUI;
        SuperCanvasRef DrumUI;
        SuperCanvasRef Sequencer1UI;
        SuperCanvasRef Sequencer2UI;
    public:
        
        struct SEQUENCER {
            float key = 0 ;
            float octave = 0 ;
            float gate = 0 ;
            float velocity = 0 ;
        };
        struct DRUM {
            float gate = 0 ;
        };
        struct CC {
            float value = 0;
        };
        
        struct DATA {
            float BPM = 0;
            SEQUENCER sequencer[2];
            DRUM drum[16];
            CC cc[16];
            DATA(){}
            DATA(JsonTree data)
            {}
        };
        
        void midiListener(midi::Message msg);
        
        midi::Input mInput;
        
        DATA data;
        
        virtual ~BeatStepPro(){
            data.~DATA();
            mMainWinCtx = nullptr;
        }
        static int COUNT;
        
        static int TICK_PER_QUARTER_NOTE;
        static int S_PER_MINUTE;
        static float S_PER_QUARTER_NOTE;

        typedef std::shared_ptr<class BeatStepPro> BeatStepProRef;
        
        static BeatStepProRef create( const std::string name, vec2 origin, gl::Context * mMainWinCtx, JsonTree data = JsonTree())
        {
            BeatStepPro::COUNT++;
            return BeatStepProRef( new BeatStepPro( name, data, origin, vec2(WIDTH*0.8, 280), mMainWinCtx ) );
        }
        
        virtual JsonTree getData() override {
            {
                JsonTree obj = ModuleCommon::getData();
                obj.addChild(JsonTree("type", "BeatStepPro"));
                JsonTree sub = JsonTree::makeObject("data");
                obj.pushBack(sub);
                return obj;
            }
        };
        void setupUI() override;
        void setup() override;
        void update() override;
        void draw() override;
    };
    
    typedef std::shared_ptr<class BeatStepPro> BeatStepProRef;
}


#endif /* BeatStepPro_hpp */
