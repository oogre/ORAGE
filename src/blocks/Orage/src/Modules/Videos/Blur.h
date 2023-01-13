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
#include "DataRange.h"

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    class Blur : public ModuleVideo{

        struct DATA {
            RANGE amountX;
            RANGE amountY;
            DATA():
                amountX(0.0f, 0.0f, 20.0f),
                amountY(0.0f, 0.0f, 20.0f)
            {};
            DATA(JsonTree data):
                amountX(data.getChild("amountX")),
                amountY(data.getChild("amountY"))
            {}
        };
        DATA data;
        
        struct S_DATA {
            float amountX;
            float amountY;
            int blurActive;
            float width = 1.0f/FBO_WIDTH;
            float height = 1.0f/FBO_HEIGHT;
        };
        S_DATA sData;
        
        
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
                sub.addChild(data.amountX.getData("amountX", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("X Blur Limiter"))));
                sub.addChild(data.amountY.getData("amountY", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("Y Blur Limiter"))));
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
