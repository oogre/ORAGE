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
#include "DataRange.h"

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    class Kaleidoscope : public ModuleVideo{
        
        struct DATA {
            RANGE segments;
            RANGE offset;
            RANGE rotation;
            RANGE scale;
            RANGE x;
            RANGE y;
            DATA():
                segments(6.0f, 0.f, 20.0f),
                rotation(.0f, 0.f, 1.0f),
                scale(1.0f, 1.f, 10.0f),
                x(0.5f, 0.f, 1.0f),
                y(0.5f, 0.f, 1.0f),
                offset(0.0f, 0.f, 1.0f)
            {};
            DATA(JsonTree data):
                segments(data.getChild("segments")),
                offset(data.getChild("offset")),
                rotation(data.getChild("rotation")),
                scale(data.getChild("scale")),
                x(data.getChild("x")),
                y(data.getChild("y"))
            {}
        };
        DATA data;
        
        struct S_DATA {
            float segments;
            float offset;
            float rotation;
            float scale;
            float x;
            float y;
            int kaleidoscopeActive = 0;
        };
        S_DATA sData;
        
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
                sub.addChild(data.segments.getData("segments", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("segments Limiter"))));
                sub.addChild(data.offset.getData("offset", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("offset Limiter"))));
                sub.addChild(data.rotation.getData("rotation", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("rotation Limiter"))));
                sub.addChild(data.scale.getData("scale", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("scale Limiter"))));
                sub.addChild(data.x.getData("x", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("x Limiter"))));
                sub.addChild(data.y.getData("y", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("y Limiter"))));
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
