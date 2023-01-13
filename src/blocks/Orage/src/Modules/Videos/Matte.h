//
//  Matte.hpp
//  orage
//
//  Created by Vincent Evrard on 2017-04-20.
//
//

#ifndef Matte_hpp
#define Matte_hpp

#include "ModuleVideo.h"
#include "DataRange.h"

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    class Matte : public ModuleVideo{
        gl::Context * mMainWinCtx;
        
        Matte(string name, JsonTree data, vec2 origin, vec2 size, gl::Context * mMainWinCtx);
        
        
        SliderfRef sY;
        SliderfRef sPb;
        SliderfRef sPr;
        
        struct DATA {
            RANGE sY;
            RANGE sPb;
            RANGE sPr;
            DATA():
                sY(0.0f, .0f, 1.0f),
                sPb(0.0f, -0.5f, 0.5f),
                sPr(0.0f, -0.5f, 0.5f)
            {};
            DATA(JsonTree data):
                sY(data.getChild("sY")),
                sPb(data.getChild("sPb")),
                sPr(data.getChild("sPr"))
            {}
        };
        DATA data;
        
        ColorAT<float> color = ColorA::black();
        
        ColorAT<float> YPbPr2Color(vec3 YPbPr);
        void updateColor(float a);
    public:
        static int COUNT;
        virtual ~Matte(){
            data.~DATA();
            mFbo.reset();
            sY.reset();
            sPb.reset();
            sPr.reset();
            mMainWinCtx = nullptr;
        }
        
        
        typedef std::shared_ptr<class Matte> MatteRef;
        
        static MatteRef create( const std::string name, vec2 origin, gl::Context * mMainWinCtx, JsonTree data = JsonTree())
        {
            Matte::COUNT++;
            return MatteRef( new Matte( name, data, origin, vec2(WIDTH, 155), mMainWinCtx ) );
        }
        
        gl::FboRef			mFbo;
        
        virtual JsonTree getData() override {
            {
                JsonTree obj = ModuleCommon::getData();
                obj.addChild(JsonTree("type", "Matte"));
                JsonTree sub = JsonTree::makeObject("data");
                sub.addChild(data.sY.getData("Y", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("sY Limiter"))));
                sub.addChild(data.sPb.getData("Pb", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("sPb Limiter"))));
                sub.addChild(data.sPr.getData("Pr", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("sPr Limiter"))));
                obj.pushBack(sub);
                return obj;
            }
        };
        
        void setupUI() override;
        void setup() override;
        void update() override;
    };
    
    typedef std::shared_ptr<class Matte> MatteRef;
}


#endif /* Matte_hpp */
