//
//  Kaleidoscope.hpp
//  PianoProjete
//
//  Created by Vincent Evrard on 7/11/17.
//
//

#ifndef Kaleidoscope_hpp
#define Kaleidoscope_hpp

#include "ModuleVideo.h"

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    class Kaleidoscope : public ModuleVideo{
        
        struct DATA {
            float segments = 6.0f;
            float offset = .0f;
            float rotation = 0.0f;
            float scale = 1.0f;
            float x = .5f;
            float y = .5f;
            bool kaleidoscopeActive = false;
            DATA(){};
            DATA(JsonTree data):
                segments(data.getChild("segments").getValue<float>()),
                offset(data.getChild("offset").getValue<float>()),
                rotation(data.getChild("rotation").getValue<float>()),
                scale(data.getChild("scale").getValue<float>()),
                x(data.getChild("x").getValue<float>()),
                y(data.getChild("y").getValue<float>()),
                kaleidoscopeActive(data.getChild("kaleidoscopeActive").getValue<bool>())
            {}
        };
        DATA data;
        
        gl::UboRef          dataUbo;
        gl::FboRef			mFbo, mFbo2;
        gl::GlslProgRef     mShader;
        
        
        Kaleidoscope(string name, JsonTree data, vec2 origin, vec2 size, gl::Context * mMainWinCtx);
        
    public:
        static int COUNT;
        
        virtual ~Kaleidoscope(){
            data.~DATA();
            dataUbo.reset();
            mFbo.reset();
            mShader.reset();
            mMainWinCtx = nullptr;
        }
        
        virtual void setData(int id, int elem, float nValue) override {
            switch(id){
                case 0 : data.segments = lerp(0.0f, 20.0f, nValue); break;
                case 1 : data.rotation = lerp(.0f, 1.0f, nValue); break;
                case 2 : data.scale = lerp(1.0f, 10.0f, nValue); break;
                case 3 : data.x = lerp(0.f, 1.f, nValue); break;
                case 4 : data.y = lerp(.0f, 1.0f, nValue); break;
                case 5 : data.offset = lerp(.0f, 1.0f, nValue); break;
            }
        }
        
        typedef std::shared_ptr<class Kaleidoscope> KaleidoscopeRef;
        gl::Context * mMainWinCtx;
        static KaleidoscopeRef create( const std::string name, vec2 origin, gl::Context * mMainWinCtx, JsonTree data = JsonTree())
        {
            Kaleidoscope::COUNT++;
            return KaleidoscopeRef( new Kaleidoscope( name, data, origin, vec2(WIDTH, 580), mMainWinCtx ) );
        }
        virtual JsonTree getData() override {
            {
                JsonTree obj = ModuleCommon::getData();
                obj.addChild(JsonTree("type", "Kaleidoscope"));
                JsonTree sub = JsonTree::makeObject("data");
                sub.addChild(JsonTree("segments", data.segments));
                sub.addChild(JsonTree("offset", data.offset));
                sub.addChild(JsonTree("rotation", data.rotation));
                sub.addChild(JsonTree("scale", data.scale));
                sub.addChild(JsonTree("x", data.x));
                sub.addChild(JsonTree("y", data.y));
                sub.addChild(JsonTree("kaleidoscopeActive", data.kaleidoscopeActive));
                obj.pushBack(sub);
                return obj;
            }
        };
        void setupShader() override;
        void setupUI() override;
        void setup() override;
        void update() override;
    };
    
    typedef std::shared_ptr<class Kaleidoscope> KaleidoscopeRef;
}


#endif /* Kaleidoscope_hpp */
