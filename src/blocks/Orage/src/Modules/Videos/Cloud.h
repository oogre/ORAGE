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
#include "DataRange.h"
#include "cinder/Rand.h"

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    class Cloud : public ModuleVideo{
        gl::Context * mMainWinCtx;
        float o_time = 0.0f;
        
        struct DATA {
            RANGE octave;
            RANGE sizex;
            RANGE sizey;
            RANGE move;
            RANGE redShift;
            RANGE blueShift;
            RANGE greenShift;
            RANGE redLvl;
            RANGE blueLvl;
            RANGE greenLvl;
            RANGE lvl;
            DATA():
                octave(1.0f, 1.0f, 10.0f),
                sizex(1.0f, .0f, 10.0f),
                sizey(1.0f, .0f, 10.0f),
                move(0.0f, -1.f, 1.f),
                redShift(0.0f, .0f, 10.0f),
                blueShift(0.0f, .0f, 10.0f),
                greenShift(0.0f, .0f, 10.0f),
                redLvl(1.0f, -1.0f, 1.0f),
                blueLvl(1.0f, -1.0f, 1.0f),
                greenLvl(1.0f, -1.0f, 1.0f),
                lvl(1.0f, 0.0f, 2.0f)
            {};
            
            DATA(JsonTree data):
                octave(data.getChild("octave")),
                sizex(data.getChild("sizex")),
                sizey(data.getChild("sizey")),
                move(data.getChild("move")),
                redShift(data.getChild("redShift")),
                blueShift(data.getChild("blueShift")),
                greenShift(data.getChild("greenShift")),
                redLvl(data.getChild("redLvl")),
                blueLvl(data.getChild("blueLvl")),
                greenLvl(data.getChild("greenLvl")),
                lvl(data.getChild("lvl"))
            {}
        };
        DATA data;
        
        struct S_DATA {
            float octave;
            float sizex;
            float sizey;
            float move;
            float redShift;
            float blueShift;
            float greenShift;
            float redLvl;
            float blueLvl;
            float greenLvl;
            float lvl;
            float u_time = 0.0f;
            float shiftx = Rand::randFloat(0.0f, 1.f);
            float shifty = Rand::randFloat(0.0f, 1.f);
            float alpha = Rand::randFloat(0.0f, 1.f);
            float seedXX = Rand::randFloat(-1.0f, 1.f);
            float seedYX = Rand::randFloat(-1.0f, 1.f);
            float seedZX = Rand::randFloat(-1.0f, 1.f);
            float seedXY = Rand::randFloat(-1.0f, 1.f);
            float seedYY = Rand::randFloat(-1.0f, 1.f);
            float seedZY = Rand::randFloat(-1.0f, 1.f);
            int modifierA = 0;
            int modifierB = 0;
        };
        
        S_DATA sData;
        
        
        
        gl::UboRef          dataUbo;
        gl::FboRef			mFbo;
        gl::GlslProgRef     mShader;
        
        
        
        Cloud(string name, JsonTree data, vec2 origin, vec2 size, gl::Context * mMainWinCtx);
    public:
        static int COUNT;
        virtual ~Cloud(){
            data.~DATA();
            sData.~S_DATA();
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
            
            sub.addChild(data.octave.getData("octave", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("octave Limiter"))));
            sub.addChild(data.sizex.getData("sizex", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("sizex Limiter"))));
            sub.addChild(data.sizey.getData("sizey", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("sizey Limiter"))));
            sub.addChild(data.move.getData("move", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("move Limiter"))));
            sub.addChild(data.redShift.getData("redShift", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("redShift Limiter"))));
            sub.addChild(data.blueShift.getData("blueShift", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("blueShift Limiter"))));
            sub.addChild(data.greenShift.getData("greenShift", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("greenShift Limiter"))));
            sub.addChild(data.redLvl.getData("redLvl", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("redLvl Limiter"))));
            sub.addChild(data.blueLvl.getData("blueLvl", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("blueLvl Limiter"))));
            sub.addChild(data.greenLvl.getData("greenLvl", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("greenLvl Limiter"))));
            sub.addChild(data.lvl.getData("lvl", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("lvl Limiter"))));
            
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
