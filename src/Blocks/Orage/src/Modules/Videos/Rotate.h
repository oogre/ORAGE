//
//  Rotate.hpp
//  PianoProjete
//
//  Created by Vincent Evrard on 2017-06-10.
//
//

#ifndef Rotate_hpp
#define Rotate_hpp

#include "ModuleVideo.h"

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    class Rotate : public ModuleVideo{
        gl::Context * mMainWinCtx;
        
        float posX   = 0.5f;
        float posY   = 0.5f;
        float posZ   = 0.0f;
        
        float rX   = 0.0f;
        float rY   = 0.0f;
        float rZ   = 0.0f;
        
        static float PI;
        
        gl::FboRef			mFbo, mFbo2;
        Rotate(string name, JsonTree data, vec2 origin, vec2 size, gl::Context * mMainWinCtx);
        
    public:
        static int COUNT;
        virtual ~Rotate(){
            // data.~DelayData();
            mFbo.reset();
            mMainWinCtx = nullptr;
        }
        
        typedef std::shared_ptr<class Rotate> RotateRef;
        
        static RotateRef create( const std::string name, vec2 origin, gl::Context * mMainWinCtx, JsonTree data = JsonTree())
        {
            Rotate::COUNT++;
            return RotateRef( new Rotate( name, data, origin, vec2(WIDTH, 580), mMainWinCtx ) );
        }
        
        virtual JsonTree getData() override {
            {
                JsonTree obj = ModuleCommon::getData();
                obj.addChild(JsonTree("type", "Rotate"));
                JsonTree sub = JsonTree::makeObject("data");
                sub.addChild(JsonTree("posX", posX));
                sub.addChild(JsonTree("posY", posY));
                sub.addChild(JsonTree("posZ", posZ));
                sub.addChild(JsonTree("rX", rX));
                sub.addChild(JsonTree("rsY", rY));
                sub.addChild(JsonTree("rZ", rZ));
                obj.pushBack(sub);
                return obj;
            }
        };
        void setupShader() override;
        void setupUI() override;
        void setup() override;
        void update() override;
    };
    
    typedef std::shared_ptr<class Rotate> RotateRef;
}


#endif /* Rotate_hpp */
