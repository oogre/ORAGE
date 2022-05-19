//
//  Lfos.hpp
//  orage
//
//  Created by Vincent Evrard on 2017-04-22.
//
//

#ifndef CDelay_hpp
#define CDelay_hpp

#include "ModuleBase.h"
#include "cinder/Perlin.h"

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    

    
    
    class CDelay : public ModuleCommon{
        
        gl::Context * mMainWinCtx;
        float oldTime = getElapsedSeconds();
        float timeCounter = 0.0f;
        
        CDelay(string name, JsonTree data, vec2 origin, vec2 size, gl::Context * mMainWinCtx);
        static const int maxMemoryLength = 300;
        float memory[CDelay::maxMemoryLength] ;
        int WCursor = 0;
        int RCursor = 0;
    public:
        
        
        struct DATA {
            float value;
            float length;
            float output;
            DATA():
                value(0.0f),
                length(0.0f),
                output(0.0f)
            {}
            DATA(JsonTree data):
                value(data.getChild("value").getValue<float>()),
                length(data.getChild("length").getValue<float>()),
                output(data.getChild("output").getValue<float>())
            {}
        };
        
        DATA data;
        
        virtual ~CDelay(){
            data.~DATA();
            mMainWinCtx = nullptr;
        }
        static int COUNT;
        
        typedef std::shared_ptr<class CDelay> CDelayRef;
        
        static CDelayRef create( const std::string name, vec2 origin, gl::Context * mMainWinCtx, JsonTree data = JsonTree())
        {
            CDelay::COUNT++;
            return CDelayRef( new CDelay( name, data, origin, vec2(WIDTH*0.8, 280), mMainWinCtx ) );
        }
        
        virtual JsonTree getData() override {
            {
                JsonTree obj = ModuleCommon::getData();
                obj.addChild(JsonTree("type", "CDelay"));
                JsonTree sub = JsonTree::makeObject("data");
                sub.addChild(JsonTree("value", data.value));
                sub.addChild(JsonTree("length", data.length));
                sub.addChild(JsonTree("output", data.output));
                obj.pushBack(sub);
                return obj;
            }
        };
        void setupUI() override;
        void setup() override;
        void update() override;
    };
    
    typedef std::shared_ptr<class CDelay> CDelayRef;
}

#endif /* CDelay_hpp */
