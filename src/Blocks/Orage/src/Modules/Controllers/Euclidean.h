//
//  Lfos.hpp
//  orage
//
//  Created by Vincent Evrard on 2017-04-22.
//
//

#ifndef Euclidean_hpp
#define Euclidean_hpp

#include "ModuleBase.h"
#include "cinder/Perlin.h"

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    
    class Euclidean : public ModuleCommon{
        
        gl::Context * mMainWinCtx;
//        float oldTime = getElapsedSeconds();
//        float timeCounter = 0;
        
        Euclidean(string name, JsonTree data, vec2 origin, vec2 size, gl::Context * mMainWinCtx);
        
        static const int maxRythmLength = 16;
//        bool rythm[Euclidean::maxRythmLength];
        list<bool> rythm;
        int oldSteps = 0;
        int oldBeats = 0;
        float oldPhase = 0;
        
        int oldStepCursor = 0;
        int stepCursor = 0;
        int oldTime = 0;
    public:
        
        
        struct DATA {
            float time;
            float steps;
            float beats;
            float phase;
            float out1;
            float out2;
            float click;
            DATA():
                time(0.0f),
                steps(1.0f),
                beats(1.0f),
                phase(0.0f),
                out1(0.0f),
                out2(0.0f),
                click(0.0f)
            {}
            DATA(JsonTree data):
                time(data.getChild("time").getValue<float>()),
                steps(data.getChild("steps").getValue<float>()),
                beats(data.getChild("beats").getValue<float>()),
                phase(data.getChild("phase").getValue<float>()),
                out1(data.getChild("out1").getValue<float>()),
                out2(data.getChild("out2").getValue<float>()),
                click(data.getChild("click").getValue<float>())
            {}
        };
        
        DATA data;
        
        virtual ~Euclidean(){
            data.~DATA();
            mMainWinCtx = nullptr;
        }
        static int COUNT;
        
        typedef std::shared_ptr<class Euclidean> EuclideanRef;
        
        static EuclideanRef create( const std::string name, vec2 origin, gl::Context * mMainWinCtx, JsonTree data = JsonTree())
        {
            Euclidean::COUNT++;
            return EuclideanRef( new Euclidean( name, data, origin, vec2(WIDTH*0.8, 280), mMainWinCtx ) );
        }
        list<bool> buildRythm(int steps, int beats, float phase);
            
        virtual JsonTree getData() override {
            {
                JsonTree obj = ModuleCommon::getData();
                obj.addChild(JsonTree("type", "Euclidean"));
                JsonTree sub = JsonTree::makeObject("data");
                sub.addChild(JsonTree("time", data.time));
                sub.addChild(JsonTree("steps", data.steps));
                sub.addChild(JsonTree("beats", data.beats));
                sub.addChild(JsonTree("phase", data.phase));
                sub.addChild(JsonTree("out1", data.out1));
                sub.addChild(JsonTree("out2", data.out2));
                sub.addChild(JsonTree("click", data.click));
                obj.pushBack(sub);
                return obj;
            }
        };
        void setupUI() override;
        void setup() override;
        void update() override;
    };
    
    typedef std::shared_ptr<class Euclidean> EuclideanRef;
}

#endif /* Euclidean_hpp */
