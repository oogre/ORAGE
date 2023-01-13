//
//  TintCorrector.hpp
//  PianoProjete
//
//  Created by Vincent Evrard on 2017-07-06.
//
//

#ifndef TintCorrector_hpp
#define TintCorrector_hpp

#include "ModuleVideo.h"
#include "DataRange.h"


using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    class TintCorrector : public ModuleVideo{
        gl::Context * mMainWinCtx;
        
        struct DATA {
            RANGE hue;
            RANGE sat;
            RANGE light;
            DATA() :
                hue(0.0f, -1.0f, 1.0f),
                sat(0.0f, -1.0f, 1.0f),
                light(0.0f, -1.0f, 1.0f)
            {};
            DATA(JsonTree data):
                hue(data.getChild("hue")),
                sat(data.getChild("sat")),
                light(data.getChild("light"))
            {}
        } ;
        DATA data;
        
        struct S_DATA {
            float hue;
            float sat;
            float light;
            int modifier = 0;
        } ;
        S_DATA sData;
        
        
        
        gl::UboRef          dataUbo;
        gl::FboRef			mFbo, mFbo2;
        gl::GlslProgRef     mShader;
        
        TintCorrector(string name, JsonTree data, vec2 origin, vec2 size, gl::Context * mMainWinCtx);
        
    public:
        static int COUNT;
        
        virtual ~TintCorrector(){
            data.~DATA();
            sData.~S_DATA();
            dataUbo.reset();
            mFbo.reset();
            mShader.reset();
            mMainWinCtx = nullptr;
        }
        
        typedef std::shared_ptr<class TintCorrector> TintCorrectorRef;
        
        static TintCorrectorRef create( const std::string name, vec2 origin, gl::Context * mMainWinCtx, JsonTree data = JsonTree())
        {
            TintCorrector::COUNT++;
            return TintCorrectorRef( new TintCorrector( name, data, origin, vec2(WIDTH, 580), mMainWinCtx ) );
        }
        
        virtual JsonTree getData() override {
            {
                JsonTree obj = ModuleCommon::getData();
                obj.addChild(JsonTree("type", "TintCorrector"));
                JsonTree sub = JsonTree::makeObject("data");
                sub.addChild(data.hue.getData("hue", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("hue Limiter"))));
                sub.addChild(data.sat.getData("sat", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("sat Limiter"))));
                sub.addChild(data.light.getData("light", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("light Limiter"))));
                obj.pushBack(sub);
                return obj;
            }
        };
        void setupShader() override;
        void setupUI() override;
        void setup() override;
        void update() override;
    };
    
    typedef std::shared_ptr<class TintCorrector> TintCorrectorRef;
}



#endif /* TintCorrector_hpp */
