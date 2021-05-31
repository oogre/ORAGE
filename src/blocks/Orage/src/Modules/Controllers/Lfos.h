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
        
        virtual void setData(int id, int elem, float nValue) override {
            switch(id){
                case 0 : data.active = nValue == 0 ? !data.active : data.active; break;
                case 1 : data.follow = nValue == 0 ? !data.follow : data.follow; break;
                case 2 : data.reverse = nValue == 0 ? !data.reverse : data.reverse; break;
                case 3 : data.BPM = lerp(30.f, 300.0f, nValue); break;
                case 4 : data.clk_multi = lerp(0.0f, 8.0f, nValue); break;
                case 5 : data.clk_div = lerp(0.0f, 8.0f, nValue); break;
                case 6 : data.delay = lerp(.0f, 1.0f, nValue); break;
                case 7 : data.exp = lerp(1.0f, 9.0f, nValue); break;
            }
        }
        virtual void incData(int id, float nValue) override {
            switch(id){
                case 0 : data.active = !data.active; break;
                case 1 : data.follow = !data.follow; break;
                case 2 : data.reverse = !data.reverse; break;
                case 3 : data.BPM = max(min(data.BPM + nValue, 300.0f), 30.0f); break;
                case 4 : data.clk_multi = max(min(data.clk_multi + nValue*0.1f, 8.0f), 0.0f); break;
                case 5 : data.clk_div = max(min(data.clk_div + nValue*0.1f, 8.0f), 0.0f); break;
                case 6 : data.delay = max(min(data.delay + nValue*0.1f, 1.0f), 0.0f); break;
                case 7 : data.exp = max(min(data.exp + nValue*0.1f, 9.0f), 1.0f); break;
            }
        }
        
        virtual void resetData(int id) override {
            switch(id){
                case 3 : data.BPM = 114.0f; break;
                case 4 : data.clk_multi = 0.0f; break;
                case 5 : data.clk_div = 0.0f; break;
                case 6 : data.delay = 0.5f; break;
                case 7 : data.exp = 5; break;
            }
        }
        
        
        struct DATA {
            bool active;
            bool follow;
            bool reverse;
            float reset;
            float oldReset;
            float delay;
            float BPM;
            float clk_multi;
            float clk_div;
            float exp;
            float saw;
            float sine;
            float sine2;
            float rect;
            DATA():
                reset(0.0f),
                oldReset(0.0f),
                active(true),
                follow(true),
                reverse(false),
                delay(0.5f),
                BPM(114.0f),
                clk_multi(0.0f),
                clk_div(0.0f),
                exp(5),
                saw(0.f),
                sine(0.f),
                sine2(1.f),
                rect(0.f)
            {}
            DATA(JsonTree data):
                active(data.getChild("active").getValue<bool>()),
                follow(data.getChild("follow").getValue<bool>()),
                reverse(data.getChild("reverse").getValue<bool>()),
                delay(data.getChild("delay").getValue<float>()),
                BPM(data.getChild("BPM").getValue<float>()),
                clk_multi(data.getChild("clk_multi").getValue<float>()),
                clk_div(data.getChild("clk_div").getValue<float>()),
                exp(data.getChild("exp").getValue<float>()),
                saw(data.getChild("saw").getValue<float>()),
                sine(data.getChild("sine").getValue<float>()),
                sine2(data.getChild("sine2").getValue<float>()),
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
                sub.addChild(JsonTree("follow", data.follow));
                sub.addChild(JsonTree("reverse", data.reverse));
                sub.addChild(JsonTree("delay", data.delay));
                sub.addChild(JsonTree("BPM", data.BPM));
                sub.addChild(JsonTree("clk_multi", data.clk_multi));
                sub.addChild(JsonTree("clk_div", data.clk_div));
                sub.addChild(JsonTree("exp", data.exp));
                sub.addChild(JsonTree("saw", data.saw));
                sub.addChild(JsonTree("sine", data.sine));
                sub.addChild(JsonTree("sine2", data.sine2));
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
