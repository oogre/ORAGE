//
//  Cloud.hpp
//  ORAGE2
//
//  Created by Vincent Evrard on 2017-04-27.
//
//

#ifndef Cloud_hpp
#define Cloud_hpp


#include "ModuleVideo.h"

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    class Cloud : public ModuleVideo{
        gl::Context * mMainWinCtx;
        float o_time = 0.0f;
        struct DATA {
            float shiftx = 0.f;
            float shifty = 0.f;
            float alpha = 0.f;
            float redShift = 0.f;
            float blueShift = 0.f;
            float greenShift = 0.f;
            float redLvl = 1.f;
            float blueLvl = 1.f;
            float greenLvl = 1.f;
            float lvl = 1.f;
            float move = 0.f;
            float u_time = 0.0f;
            float sizex = 1.0f;
            float sizey = 1.0f;
            float octave = 1.0f;
            float seedXX = 0.f;
            float seedYX = 0.f;
            float seedZX = 0.f;
            float seedXY = 0.f;
            float seedYY = 0.f;
            float seedZY = 0.f;
            int modifierA = 0;
            int modifierB = 0;
            DATA(){};
            DATA(JsonTree data):
                shiftx(data.getChild("shiftx").getValue<float>()),
                shifty(data.getChild("shifty").getValue<float>()),
                alpha(data.getChild("alpha").getValue<float>()),
                redShift(data.getChild("redShift").getValue<float>()),
                blueShift(data.getChild("blueShift").getValue<float>()),
                greenShift(data.getChild("greenShift").getValue<float>()),
                lvl(data.getChild("lvl").getValue<float>()),
                move(data.getChild("move").getValue<float>()),
                u_time(data.getChild("u_time").getValue<float>()),
                sizex(data.getChild("sizex").getValue<float>()),
                sizey(data.getChild("sizey").getValue<float>()),
                octave(data.getChild("octave").getValue<float>()),
                seedXX(data.getChild("seedXX").getValue<float>()),
                seedYX(data.getChild("seedYX").getValue<float>()),
                seedZX(data.getChild("seedZX").getValue<float>()),
                seedXY(data.getChild("seedXY").getValue<float>()),
                seedYY(data.getChild("seedYY").getValue<float>()),
                seedZY(data.getChild("seedZY").getValue<float>()),
                modifierA(data.getChild("modifierA").getValue<int>()),
                modifierB(data.getChild("modifierB").getValue<int>())
            {}
        };
        DATA data;
        
        gl::UboRef          dataUbo;
        gl::FboRef			mFbo;
        gl::GlslProgRef     mShader;
        
        
        
        Cloud(string name, JsonTree data, vec2 origin, vec2 size, gl::Context * mMainWinCtx);
    public:
        static int COUNT;
        virtual ~Cloud(){
            data.~DATA();
            dataUbo.reset();
            mFbo.reset();
            mShader.reset();
            mMainWinCtx = nullptr;
        }
        
        virtual void setData(int id, int elem, float nValue) override {
            switch(id){
                case 0 : data.octave = lerp(1.0f, 10.0f, nValue); break;
                case 1 : data.sizex = lerp(.0f, 10.0f, nValue); break;
                case 2 : data.sizey = lerp(.0f, 10.0f, nValue); break;
                case 3 : data.move = lerp(-1.f, 1.f, nValue); break;
                case 4 : data.redShift = lerp(.0f, 10.0f, nValue); break;
                case 5 : data.blueShift = lerp(.0f, 10.0f, nValue); break;
                case 6 : data.greenShift = lerp(.0f, 10.0f, nValue); break;
                case 7 : data.redLvl = lerp(-1.0f, 1.0f, nValue); break;
                case 8 : data.blueLvl = lerp(-1.0f, 1.0f, nValue); break;
                case 9 : data.greenLvl = lerp(-1.0f, 1.0f, nValue); break;
                case 10 : data.lvl = lerp(.0f, 2.0f, nValue); break;
            }
        }
        /*
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
        */
        typedef std::shared_ptr<class Cloud> CloudRef;
        
        static CloudRef create( const std::string name, vec2 origin, gl::Context * mMainWinCtx, JsonTree data = JsonTree())
        {
            Cloud::COUNT++;
            return CloudRef( new Cloud( name, data, origin, vec2(WIDTH, 580),  mMainWinCtx ) );
        }
        
        virtual JsonTree getData() override {
            JsonTree obj = ModuleCommon::getData();
            obj.addChild(JsonTree("type", "Cloud"));
            JsonTree sub = JsonTree::makeObject("data");
            sub.addChild(JsonTree("shiftx", data.shiftx));
            sub.addChild(JsonTree("shifty", data.shifty));
            sub.addChild(JsonTree("alpha", data.alpha));
            sub.addChild(JsonTree("redShift", data.redShift));
            sub.addChild(JsonTree("blueShift", data.blueShift));
            sub.addChild(JsonTree("greenShift", data.greenShift));
            sub.addChild(JsonTree("redLvl", data.redLvl));
            sub.addChild(JsonTree("blueLvl", data.blueLvl));
            sub.addChild(JsonTree("greenLvl", data.greenLvl));
            sub.addChild(JsonTree("lvl", data.lvl));
            sub.addChild(JsonTree("move", data.move));
            sub.addChild(JsonTree("u_time", data.u_time));
            sub.addChild(JsonTree("sizex", data.sizex));
            sub.addChild(JsonTree("sizey", data.sizey));
            sub.addChild(JsonTree("octave", data.octave));
            sub.addChild(JsonTree("seedXX", data.seedXX));
            sub.addChild(JsonTree("seedYX", data.seedYX));
            sub.addChild(JsonTree("seedZX", data.seedZX));
            sub.addChild(JsonTree("seedXY", data.seedXY));
            sub.addChild(JsonTree("seedYY", data.seedYY));
            sub.addChild(JsonTree("seedZY", data.seedZY));
            sub.addChild(JsonTree("modifierA", data.modifierA));
            sub.addChild(JsonTree("modifierB", data.modifierB));
            obj.pushBack(sub);
            return obj;
        };
            
        void setupShader() override;
        void setupUI() override;
        void setup() override;
        void update() override;
    };
    
    typedef std::shared_ptr<class Cloud> CloudRef;
}


#endif /* Cloud_hpp */
