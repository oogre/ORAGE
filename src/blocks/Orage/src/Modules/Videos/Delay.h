//
//  Delay.hpp
//  PianoProjete
//
//  Created by Vincent Evrard on 2017-06-01.
//
//

#ifndef Delay_hpp
#define Delay_hpp

#include "ModuleVideo.h"

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    class Delay : public ModuleVideo{
        gl::Context * mMainWinCtx;
        static const int MAX_TEX = 10;
        static const int MAX_IMG_PER_TEX = 12;
        static const int MAX_BUFFER = MAX_TEX * MAX_IMG_PER_TEX;
        float bufferLen = 1.f;
        float cursor = 0.f;
        struct DATA {
            int bufferLen = 1;
            bool modifierActive = false;
            int cursor = 0 ;
            float part = 1.0f/MAX_IMG_PER_TEX;
            float width = 1.0f/(FBO_WIDTH*MAX_IMG_PER_TEX);
            float height = 1.0f/FBO_HEIGHT;
            DATA(){};
            DATA(JsonTree data):
                bufferLen(data.getChild("bufferLen").getValue<int>()),
                modifierActive(data.getChild("modifierActive").getValue<bool>())
            {}
        };
        DATA data;
        CameraPersp         CAM2 = CameraPersp(ModuleVideo::FBO_WIDTH * MAX_IMG_PER_TEX, ModuleVideo::FBO_HEIGHT, -60.0f, 1, 1000 );
        gl::UboRef          dataUbo;
        gl::FboRef			mFbo, mFbo2;
        vector<gl::FboRef>  mFboBuffer;
        gl::GlslProgRef     mShader;
        
        cinder::gl::TextureRef    testTex;

        Delay(string name, JsonTree data, vec2 origin, vec2 size, gl::Context * mMainWinCtx);
        
    public:
        static int COUNT;
        virtual ~Delay(){
            data.~DATA();
            dataUbo.reset();
            mFbo.reset();
            mFbo2.reset();
            mShader.reset();
            mMainWinCtx = nullptr;
            mFboBuffer.clear();
        }
        
        virtual void setData(int id, int elem, float nValue) override {
            switch(id){
                case 0 : bufferLen = lerp(1, 120, nValue); break;
            }
        }
        
        typedef std::shared_ptr<class Delay> DelayRef;
        
        static DelayRef create( const std::string name, vec2 origin, gl::Context * mMainWinCtx, JsonTree data = JsonTree())
        {
            Delay::COUNT++;
            return DelayRef( new Delay( name, data, origin, vec2(WIDTH, 580), mMainWinCtx ) );
        }
        
        virtual JsonTree getData() override {
            {
                JsonTree obj = ModuleCommon::getData();
                obj.addChild(JsonTree("type", "Delay"));
                JsonTree sub = JsonTree::makeObject("data");
                sub.addChild(JsonTree("bufferLen", data.bufferLen));
                sub.addChild(JsonTree("modifierActive", data.modifierActive));
                obj.pushBack(sub);
                return obj;
            }
        };
        void setupShader() override;
        void setupUI() override;
        void setup() override;
        void update() override;
    };
    
    typedef std::shared_ptr<class Delay> DelayRef;
}


#endif /* Delay_hpp */
