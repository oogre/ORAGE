//
//  Blur.hpp
//  PianoProjete
//
//  Created by Vincent Evrard on 2017-06-10.
//
//

#ifndef Blur_hpp
#define Blur_hpp

#include "ModuleVideo.h"

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    class Blur : public ModuleVideo{

        struct DATA {
            float amountX = .0f;
            float amountY = .0f;
            bool blurActive = false;
            float width = 1.0f/FBO_WIDTH;
            float height = 1.0f/FBO_HEIGHT;
            DATA(){};
            DATA(JsonTree data):
                amountX(data.getChild("amountX").getValue<float>()),
                amountY(data.getChild("amountY").getValue<float>()),
                blurActive(data.getChild("blurActive").getValue<bool>())
            {}
        };
        DATA data;
        
        
        gl::UboRef          dataUbo;
        gl::FboRef			mFbo, mFbo2;
        gl::GlslProgRef     mShader;
        
        
        Blur(string name, JsonTree data, vec2 origin, vec2 size, gl::Context * mMainWinCtx);
        
    public:
        static int COUNT;
        
        virtual ~Blur(){
            data.~DATA();
            dataUbo.reset();
            mFbo.reset();
            mShader.reset();
            mMainWinCtx = nullptr;
        }
        
        
        virtual void setData(int id, int elem, float nValue) override {
            switch(id){
                case 0 : data.amountX = lerp(.0f, 20.f, nValue); break;
                case 1 : data.amountY = lerp(.0f, 20.f, nValue); break;
            }
        }
        
        
        typedef std::shared_ptr<class Blur> BlurRef;
        gl::Context * mMainWinCtx;
        static BlurRef create( const std::string name, vec2 origin, gl::Context * mMainWinCtx, JsonTree data = JsonTree())
        {
            Blur::COUNT++;
            return BlurRef( new Blur( name, data, origin, vec2(WIDTH, 580), mMainWinCtx ) );
        }
        
        virtual JsonTree getData() override {
            {
                JsonTree obj = ModuleCommon::getData();
                obj.addChild(JsonTree("type", "Blur"));
                JsonTree sub = JsonTree::makeObject("data");
                sub.addChild(JsonTree("amountX", data.amountX));
                sub.addChild(JsonTree("amountY", data.amountY));
                sub.addChild(JsonTree("blurActive", data.blurActive));
                obj.pushBack(sub);
                return obj;
            }
        };
        void setupShader() override;
        void setupUI() override;
        void setup() override;
        void update() override;
    };
    
    typedef std::shared_ptr<class Blur> BlurRef;
}


#endif /* Blur_hpp */
