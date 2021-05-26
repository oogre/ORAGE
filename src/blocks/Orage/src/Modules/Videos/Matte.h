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
        ColorAT<float> color = ColorA::black();
        vec3 YPbPr = vec3(0.f, 0.f, 0.f);
        vec3 YPbPrOld = vec3(0.f, 0.f, 0.f);
        ColorAT<float> YPbPr2Color(vec3 YPbPr);
        void updateColor(float a);
    public:
        static int COUNT;
        virtual ~Matte(){
            mFbo.reset();
            sY.reset();
            sPb.reset();
            sPr.reset();
            mMainWinCtx = nullptr;
        }
        
        virtual void setData(int id, int elem, float nValue) override {
            switch(id){
                case 0 : YPbPr.x = lerp(.0f, 1.f, nValue); break;
                case 1 : YPbPr.y = lerp(-.5f, .5f, nValue); break;
                case 2 : YPbPr.z = lerp(-.5f, .5f, nValue); break;
            }
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
                sub.addChild(JsonTree("sY", YPbPr.x));
                sub.addChild(JsonTree("sPb", YPbPr.y));
                sub.addChild(JsonTree("sPr", YPbPr.z));
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
