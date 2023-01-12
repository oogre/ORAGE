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
#include "DataRange.h"

#define PI 3.14159265359

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    class Rotate : public ModuleVideo{
        gl::Context * mMainWinCtx;
        
        struct DATA {
            RANGE posX;
            RANGE posY;
            RANGE posZ;
            
            RANGE rX;
            RANGE rY;
            RANGE rZ;
           
            DATA():
                posX(0.5f, 0.0f, 1.0f),
                posY(0.5f, 0.0f, 1.0f),
                posZ(0.0f, -0.25f, 0.25f),
            
                rX(0.0f, 0.0f, 2 * PI),
                rY(0.0f, 0.0f, 2 * PI),
                rZ(0.0f, 0.0f, 2 * PI)
            {}
            DATA(JsonTree data):
                posX(data.getChild("posX")),
                posY(data.getChild("posY")),
                posZ(data.getChild("posZ")),
                rX(data.getChild("rX")),
                rY(data.getChild("rY")),
                rZ(data.getChild("rZ"))
            {}
        };
        DATA data;
        
        
        gl::FboRef			mFbo, mFbo2;
        Rotate(string name, JsonTree data, vec2 origin, vec2 size, gl::Context * mMainWinCtx);
        
    public:
        static int COUNT;
        virtual ~Rotate(){
            data.~DATA();
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
                
                sub.addChild(data.posX.getData("posX", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("tX Limiter"))));
                sub.addChild(data.posY.getData("posY", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("tY Limiter"))));
                sub.addChild(data.posZ.getData("posZ", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("tZ Limiter"))));
                
                sub.addChild(data.rX.getData("rX", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("rX Limiter"))));
                sub.addChild(data.rY.getData("rY", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("rY Limiter"))));
                sub.addChild(data.rZ.getData("rZ", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("rZ Limiter"))));
                
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
