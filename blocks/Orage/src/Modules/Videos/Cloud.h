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
