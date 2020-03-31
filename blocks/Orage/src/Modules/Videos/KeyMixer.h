//
//  KeyMixer.hpp
//  orage
//
//  Created by Vincent Evrard on 2017-04-23.
//
//

#ifndef KeyMixer_hpp
#define KeyMixer_hpp


#include "ModuleVideo.h"

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    class KeyMixer : public ModuleVideo{
        gl::Context * mMainWinCtx;
        map<char, gl::Texture2dRef> oldInputs;
        TextureViewRef inputA;
        TextureViewRef inputB;
        TextureViewRef inputC;
        gl::FboRef			mFbo;
        gl::GlslProgRef     mShader;

        KeyMixer(string name, JsonTree data, vec2 origin, vec2 size, gl::Context * mMainWinCtx);
        
    public:
        virtual ~KeyMixer(){
            inputA.reset();
            inputB.reset();
            inputC.reset();
            auto itin = oldInputs.begin();
            while(itin != oldInputs.end()){
                itin->second.reset();
                itin++;
            }
            oldInputs.clear();
            mFbo.reset();
            mShader.reset();
            mMainWinCtx = nullptr;
        }
        static int COUNT;
        
        typedef std::shared_ptr<class KeyMixer> KeyMixerRef;
        
        static KeyMixerRef create( const std::string name, vec2 origin, gl::Context * mMainWinCtx, JsonTree data = JsonTree())
        {
            KeyMixer::COUNT++;
            return KeyMixerRef( new KeyMixer( name, data, origin, vec2(WIDTH*2, 500), mMainWinCtx ) );
        }
        virtual JsonTree getData() override {
            JsonTree obj = ModuleCommon::getData();
            obj.addChild(JsonTree("type", "KeyMixer"));
            return obj;
        };
        void setupShader() override;
        void setupUI() override;
        void setup() override;
        void update() override;
    };
    
    typedef std::shared_ptr<class KeyMixer> KeyMixerRef;
}


#endif /* KeyMixer_hpp */
