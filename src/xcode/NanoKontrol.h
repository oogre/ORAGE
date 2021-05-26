//
//  NanoKontrol.hpp
//  ORAGE
//
//  Created by Vincent Evrard on 30/09/20.
//

#ifndef NanoKontrol_hpp
#define NanoKontrol_hpp

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
    class NanoKontrol : public ModuleCommon{
        
        typedef const std::function<void(int,int,int)> CallbackType;
        vector<CallbackType> callbacks;
        gl::Context * mMainWinCtx;
        
        NanoKontrol(string name, JsonTree data, vec2 origin, vec2 size, gl::Context * mMainWinCtx);
        
        std::string status;
        double lastTickAt = 0;
        double averageDeltaTick = 0 ;
        static const int ccLEn = 16;
        
    public:
        
        struct CC {
            float value = 0;
        };
        
        struct DATA {
            CC cc[18];
            DATA(){}
            DATA(JsonTree data)
            {}
        };
        
        void midiListener(midi::Message msg);
        
        midi::Input mInput;
        
        DATA data;
        
        virtual ~NanoKontrol(){
            data.~DATA();
            mMainWinCtx = nullptr;
        }
        static int COUNT;
        
        typedef std::shared_ptr<class NanoKontrol> NanoKontrolRef;
        
        static NanoKontrolRef create( const std::string name, vec2 origin, gl::Context * mMainWinCtx, JsonTree data = JsonTree())
        {
            NanoKontrol::COUNT++;
            return NanoKontrolRef( new NanoKontrol( name, data, origin, vec2(WIDTH*0.8, 280), mMainWinCtx ) );
        }
        
        virtual JsonTree getData() override {
            {
                JsonTree obj = ModuleCommon::getData();
                obj.addChild(JsonTree("type", "NanoKontrol"));
                JsonTree sub = JsonTree::makeObject("data");
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
    
    typedef std::shared_ptr<class NanoKontrol> NanoKontrolRef;
}


#endif /* NanoKontrol_hpp */
