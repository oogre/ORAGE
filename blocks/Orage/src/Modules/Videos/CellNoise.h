//
//  CellNoise.hpp
//  ORAGE2
//
//  Created by Vincent Evrard on 2017-04-27.
//
//

#ifndef CellNoise_hpp
#define CellNoise_hpp


#include "ModuleVideo.h"

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    class CellNoise : public ModuleVideo{
        gl::Context * mMainWinCtx;
        
        struct DATA {
            float movex = .0f; //gain
            float movey = .0f; //gain
            float size = 1.0f; //gain
            float square = 1.0f; //gain
            float power = 1.0f;
            DATA(){};
            DATA(JsonTree data):
                movex(data.getChild("movex").getValue<float>()),
                movey(data.getChild("movey").getValue<float>()),
                size(data.getChild("size").getValue<float>()),
                square(data.getChild("square").getValue<float>()),
                power(data.getChild("power").getValue<float>())
            {}
        };
        DATA data;
        
        gl::UboRef          dataUbo;
        gl::FboRef			mFbo;
        gl::GlslProgRef     mShader;
        
        
        
        CellNoise(string name, JsonTree data, vec2 origin, vec2 size, gl::Context * mMainWinCtx);
    public:
        static int COUNT;
        
        virtual ~CellNoise(){
            data.~DATA();
            dataUbo.reset();
            mFbo.reset();
            mShader.reset();
            mMainWinCtx = nullptr;
        }
        
        typedef std::shared_ptr<class CellNoise> CellNoiseRef;
        
        static CellNoiseRef create( const std::string name, vec2 origin, gl::Context * mMainWinCtx, JsonTree data = JsonTree())
        {
            CellNoise::COUNT++;
            return CellNoiseRef( new CellNoise( name, data, origin, vec2(WIDTH, 580),  mMainWinCtx ) );
        }
        
        virtual JsonTree getData() override {
            JsonTree obj = ModuleCommon::getData();
            obj.addChild(JsonTree("type", "CellNoise"));
            JsonTree sub = JsonTree::makeObject("data");
            sub.addChild(JsonTree("movex", data.movex));
            sub.addChild(JsonTree("movey", data.movey));
            sub.addChild(JsonTree("size", data.size));
            sub.addChild(JsonTree("square", data.square));
            sub.addChild(JsonTree("power", data.power));
            obj.pushBack(sub);
            return obj;
        };
        void setupShader() override;
        void setupUI() override;
        void setup() override;
        void update() override;
    };
    
    typedef std::shared_ptr<class CellNoise> CellNoiseRef;
}


#endif /* CellNoise_hpp */
