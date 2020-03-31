//
//  Lfos.hpp
//  orage
//
//  Created by Vincent Evrard on 2017-04-22.
//
//

#ifndef Lfos_hpp
#define Lfos_hpp

#include "ModuleBase.h"
#include "cinder/Perlin.h"

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    class Lfos : public ModuleCommon{
        
        gl::Context * mMainWinCtx;
        float oldTime = getElapsedSeconds();
        float timeCounter = 0.0f;
        
        Lfos(string name, JsonTree data, vec2 origin, vec2 size, gl::Context * mMainWinCtx);
        
    public:
        Perlin p;
        struct DATA {
            bool active;
            bool reverse;
            float delay;
            float BPM;
            float clk_multi;
            float clk_div;
            float exp;
            float saw;
            float sine;
            float rect;
            DATA():
                active(true),
                reverse(false),
                delay(0.5f),
                BPM(114.0f),
                clk_multi(0.0f),
                clk_div(0.0f),
                exp(5),
                saw(0.f),
                sine(0.f),
                rect(0.f)
            {}
            DATA(JsonTree data):
                active(data.getChild("active").getValue<bool>()),
                reverse(data.getChild("reverse").getValue<bool>()),
                delay(data.getChild("delay").getValue<float>()),
                BPM(data.getChild("BPM").getValue<float>()),
                clk_multi(data.getChild("clk_multi").getValue<float>()),
                clk_div(data.getChild("clk_div").getValue<float>()),
                exp(data.getChild("exp").getValue<float>()),
                saw(data.getChild("saw").getValue<float>()),
                sine(data.getChild("sine").getValue<float>()),
                rect(data.getChild("rect").getValue<float>())
            {}
        };
        
        DATA data;
        
        virtual ~Lfos(){
            data.~DATA();
            mMainWinCtx = nullptr;
        }
        static int COUNT;
        
        typedef std::shared_ptr<class Lfos> LfosRef;
        
        static LfosRef create( const std::string name, vec2 origin, gl::Context * mMainWinCtx, JsonTree data = JsonTree())
        {
            Lfos::COUNT++;
            return LfosRef( new Lfos( name, data, origin, vec2(WIDTH*0.8, 280), mMainWinCtx ) );
        }
        
        virtual JsonTree getData() override {
            {
                JsonTree obj = ModuleCommon::getData();
                obj.addChild(JsonTree("type", "Lfos"));
                JsonTree sub = JsonTree::makeObject("data");
                sub.addChild(JsonTree("active", data.active));
                sub.addChild(JsonTree("reverse", data.reverse));
                sub.addChild(JsonTree("delay", data.delay));
                sub.addChild(JsonTree("BPM", data.BPM));
                sub.addChild(JsonTree("clk_multi", data.clk_multi));
                sub.addChild(JsonTree("clk_div", data.clk_div));
                sub.addChild(JsonTree("exp", data.exp));
                sub.addChild(JsonTree("saw", data.saw));
                sub.addChild(JsonTree("sine", data.sine));
                sub.addChild(JsonTree("rect", data.rect));
                obj.pushBack(sub);
                return obj;
            }
        };
        void setupUI() override;
        void setup() override;
        void update() override;
    };
    
    typedef std::shared_ptr<class Lfos> LfosRef;
}

#endif /* Lfos_hpp */
