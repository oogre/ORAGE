//
//  Crossfader.hpp
//  orage
//
//  Created by Vincent Evrard on 2017-04-21.
//
//

#ifndef Crossfader_hpp
#define Crossfader_hpp

#include "ModuleVideo.h"
#include "DataRange.h"

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    class Crossfader : public ModuleVideo{
        gl::Context * mMainWinCtx;
        map<char, gl::Texture2dRef> oldInputs;
        TextureViewRef inputA;
        TextureViewRef inputB;
        float crossfade = 0 ;
        float blend = 0;
        
        struct DATA {
            RANGE crossfade;
            RANGE blend;
            DATA():
                crossfade(0.0f, -1.0f, 1.0f),
                blend(0.0f, 0.0f, 15.0f)
            {};
            DATA(JsonTree data):
                crossfade(data.getChild("crossfade")),
                blend(data.getChild("blend"))
            {}
        };
        DATA data;
        
        struct S_DATA {
            float alpha0;
            float alpha1;
            float crossfade;
            float blend;
            int modifierA = 0;
            int modifierB = 0;
        };
        S_DATA sData;
        
        gl::UboRef          dataUbo;
        gl::FboRef			mFbo, mFbo2;
        gl::GlslProgRef     mShader;
        
        Crossfader(string name, JsonTree data, vec2 origin, vec2 size, gl::Context * mMainWinCtx);
        
    public:
        static int COUNT;
        
        virtual ~Crossfader(){
            data.~DATA();
            sData.~S_DATA();
            
            inputA.reset();
            inputB.reset();
            auto itin = oldInputs.begin();
            while(itin != oldInputs.end()){
                itin->second.reset();
                itin++;
            }
            oldInputs.clear();
            mFbo.reset();
            mFbo2.reset();
            mShader.reset();
            mMainWinCtx = nullptr;
        }
        
        
        typedef std::shared_ptr<class Crossfader> CrossfaderRef;
        
        static CrossfaderRef create( const std::string name, vec2 origin, gl::Context * mMainWinCtx, JsonTree data = JsonTree())
        {
            Crossfader::COUNT++;
            return CrossfaderRef( new Crossfader( name, data, origin, vec2(WIDTH*2, 500), mMainWinCtx ) );
        }
        
        
        
        virtual JsonTree getData() override {
            {
                JsonTree obj = ModuleCommon::getData();
                obj.addChild(JsonTree("type", "Crossfader"));
                JsonTree sub = JsonTree::makeObject("data");
                sub.addChild(data.crossfade.getData("crossfade", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("crossfade Limiter"))));
                sub.addChild(data.blend.getData("blend", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("blend Limiter"))));
                obj.pushBack(sub);
                return obj;
            }
        };
        
        void setupShader() override;
        void setupUI() override;
        void setup() override;
        void update() override;
    };
    
    typedef std::shared_ptr<class Crossfader> CrossfaderRef;
}

#endif /* Crossfader_hpp */
