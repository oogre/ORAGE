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
#include "DataRange.h"

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    class Resize : public ModuleVideo{
        gl::Context * mMainWinCtx;
        
        struct DATA {
            RANGE posX;
            RANGE posY;
            RANGE width;
            RANGE height;
            
            RANGE resX;
            RANGE resY;
            RANGE resW;
            RANGE resH;
            DATA():
                posX(0.0f, -1.0f, 1.0f),
                posY(0.0f, -1.0f, 1.0f),
                width(1.0f, -1.0f, 1.0f),
                height(1.0f, -1.0f, 1.0f),
                resX(0.0f, 0.0f, 1.0f),
                resY(0.0f, 0.0f, 1.0f),
                resW(1.0f, 0.0f, 1.0f),
                resH(1.0f, 0.0f, 1.0f)
            {}
            DATA(JsonTree data):
                posX(data.getChild("posX")),
                posY(data.getChild("posY")),
                width(data.getChild("width")),
                height(data.getChild("height")),
                resX(data.getChild("resX")),
                resY(data.getChild("resY")),
                resW(data.getChild("resW")),
                resH(data.getChild("resH"))
            {}
        };
        DATA data;
        
        
        gl::FboRef			mFbo, mFbo2;
        Resize(string name, JsonTree data, vec2 origin, vec2 size, gl::Context * mMainWinCtx);
        
    public:
        static int COUNT;
        virtual ~Resize(){
             data.~DATA();
            mFbo.reset();
            mMainWinCtx = nullptr;
        }
        
        virtual void setData(int id, int elem, float nValue) override {
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
                
                sub.addChild(data.resX.getData("resX", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("cX Limiter"))));
                sub.addChild(data.resY.getData("resY", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("cY Limiter"))));
                sub.addChild(data.resW.getData("resW", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("cW Limiter"))));
                sub.addChild(data.resH.getData("resH", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("cH Limiter"))));
                
                sub.addChild(data.posX.getData("posX", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("sX Limiter"))));
                sub.addChild(data.posY.getData("posY", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("sY Limiter"))));
                sub.addChild(data.width.getData("width", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("sW Limiter"))));
                sub.addChild(data.height.getData("height", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("sH Limiter"))));
                
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
