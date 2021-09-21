//
//  Resize.hpp
//  PianoProjete
//
//  Created by Vincent Evrard on 2017-06-10.
//
//

#ifndef Resize_hpp
#define Resize_hpp

#include "ModuleVideo.h"

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    class Resize : public ModuleVideo{
        gl::Context * mMainWinCtx;
        
        float posX    = 0.0f;
        float posY    = 0.0f;
        float width   = 1.0f;
        float height  = 1.0f;
        
        float resX    = 0.0f;
        float resY    = 0.0f;
        float resW   = 1.0f;
        float resH  = 1.0f;
        
        gl::FboRef			mFbo, mFbo2;
        Resize(string name, JsonTree data, vec2 origin, vec2 size, gl::Context * mMainWinCtx);
        
    public:
        static int COUNT;
        virtual ~Resize(){
            // data.~DelayData();
            mFbo.reset();
            mMainWinCtx = nullptr;
        }
        
        virtual void setData(int id, int elem, float nValue) override {
            switch(id){
                case 0 : resX = lerp(.0f, 1.0f, nValue); break;
                case 1 : resY = lerp(.0f, 1.0f, nValue); break;
                case 2 : resW = lerp(0.0f, 1.0f, nValue); break;
                case 3 : resH = lerp(.0f, 1.0f, nValue); break;
                case 4 : posX = lerp(-1.0f, 1.0f, nValue); break;
                case 5 : posY = lerp(-1.0f, 1.0f, nValue); break;
                case 6 : width = lerp(-1.0f, 1.0f, nValue); break;
                case 7 : height = lerp(-1.0f, 1.0f, nValue); break;
            }
        }
        
        typedef std::shared_ptr<class Resize> ResizeRef;
        
        static ResizeRef create( const std::string name, vec2 origin, gl::Context * mMainWinCtx, JsonTree data = JsonTree())
        {
            Resize::COUNT++;
            return ResizeRef( new Resize( name, data, origin, vec2(WIDTH, 580), mMainWinCtx ) );
        }
        
        virtual JsonTree getData() override {
            {
                JsonTree obj = ModuleCommon::getData();
                obj.addChild(JsonTree("type", "Resize"));
                JsonTree sub = JsonTree::makeObject("data");
                sub.addChild(JsonTree("posX", posX));
                sub.addChild(JsonTree("posY", posY));
                sub.addChild(JsonTree("width", width));
                sub.addChild(JsonTree("height", height));
                sub.addChild(JsonTree("resX", resX));
                sub.addChild(JsonTree("resY", resY));
                sub.addChild(JsonTree("resW", resW));
                sub.addChild(JsonTree("resH", resH));
                obj.pushBack(sub);
                return obj;
            }
        };
        void setupShader() override;
        void setupUI() override;
        void setup() override;
        void update() override;
    };
    
    typedef std::shared_ptr<class Resize> ResizeRef;
}


#endif /* Resize_hpp */
