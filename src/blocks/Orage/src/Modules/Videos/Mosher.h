//
//  Mosher.hpp
//  orage
//
//  Created by Vincent Evrard on 2017-04-20.
//
//

#ifndef Mosher_hpp
#define Mosher_hpp

#include "ModuleVideo.h"
#include "DataRange.h"

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    class Mosher : public ModuleVideo{
        gl::Context * mMainWinCtx;
        
        struct DATA {
            RANGE gainX;
            RANGE gainY;
            RANGE x;
            RANGE y;
            DATA():
                gainX(0.0f, -1.0f, 1.0f),
                gainY(0.0f, -1.0f, 1.0f),
                x(0.0f, -1.0f, 1.0f),
                y(0.0f, -1.0f, 1.0f)
            {};
            DATA(JsonTree data):
                gainX(data.getChild("gainX")),
                gainY(data.getChild("gainY")),
                x(data.getChild("x")),
                y(data.getChild("y"))
            {}
        };
        DATA data;
        
        struct S_DATA {
            float gainX;
            float gainY;
            float x;
            float y;
        };
        S_DATA sData;
        
        gl::UboRef          dataUbo;
        gl::FboRef			mFbo, mFbo2;
        gl::GlslProgRef     mShader;
        
        Mosher(string name, JsonTree data, vec2 origin, vec2 size, gl::Context * mMainWinCtx);
        
    public:
        static int COUNT;
        virtual ~Mosher(){
            data.~DATA();
            dataUbo.reset();
            mFbo.reset();
            mFbo2.reset();
            mShader.reset();
            mMainWinCtx = nullptr;
        }
        
        
        typedef std::shared_ptr<class Mosher> MosherRef;
        
        static MosherRef create( const std::string name, vec2 origin, gl::Context * mMainWinCtx, JsonTree data = JsonTree())
        {
            Mosher::COUNT++;
            return MosherRef( new Mosher( name, data, origin, vec2(WIDTH, 580), mMainWinCtx ) );
        }
        
        
        virtual JsonTree getData() override {
            {
                JsonTree obj = ModuleCommon::getData();
                obj.addChild(JsonTree("type", "Mosher"));
                JsonTree sub = JsonTree::makeObject("data");
                sub.addChild(data.gainX.getData("gainX", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("gainX Limiter"))));
                sub.addChild(data.gainY.getData("gainY", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("gainY Limiter"))));
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
    
    typedef std::shared_ptr<class Mosher> MosherRef;
}


#endif /* Mosher_hpp */
