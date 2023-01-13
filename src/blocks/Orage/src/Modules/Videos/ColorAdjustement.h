//
//  ColorAdjustement.hpp
//  orage
//
//  Created by Vincent Evrard on 2017-04-23.
//
//

#ifndef ColorAdjustement_hpp
#define ColorAdjustement_hpp



#include "ModuleVideo.h"
#include "DataRange.h"

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    class ColorAdjustement : public ModuleVideo{
        gl::Context * mMainWinCtx;
        
        struct DATA {
            RANGE bri;
            RANGE sat;
            RANGE con;
            RANGE red;
            RANGE green;
            RANGE blue;
            RANGE ssm;
            RANGE ssM;
            DATA():
                bri(1.0f, 0.0f, 2.0f),
                sat(1.0f, 0.0f, 2.0f),
                con(1.0f, 0.0f, 2.0f),
                red(0.0f, -1.0f, 1.0f),
                green(0.0f, -1.0f, 1.0f),
                blue(0.0f, -1.0f, 1.0f),
                ssm(0.0f, 0.0f, 1.0f),
                ssM(1.0f, 0.0f, 1.0f)
            {};
            DATA(JsonTree data):
                bri(data.getChild("bri")),
                sat(data.getChild("sat")),
                con(data.getChild("con")),
                red(data.getChild("red")),
                green(data.getChild("green")),
                blue(data.getChild("blue")),
                ssm(data.getChild("ssm")),
                ssM(data.getChild("ssM"))
            {}
        };
        DATA data;
        
        struct S_DATA {
            float bri;
            float sat;
            float con;
            float red;
            float green;
            float blue;
            float ssm;
            float ssM;
            int modifier = 0;
        } ;
        S_DATA sData;
        
        gl::UboRef          dataUbo;
        gl::FboRef			mFbo, mFbo2;
        gl::GlslProgRef     mShader;
        
        ColorAdjustement(string name, JsonTree data, vec2 origin, vec2 size, gl::Context * mMainWinCtx);
        
    public:
        static int COUNT;
        
        virtual ~ColorAdjustement(){
            data.~DATA();
            sData.~S_DATA();
            dataUbo.reset();
            mFbo.reset();
            mShader.reset();
            mMainWinCtx = nullptr;
        }
        
        
        typedef std::shared_ptr<class ColorAdjustement> ColorAdjustementRef;
        
        static ColorAdjustementRef create( const std::string name, vec2 origin, gl::Context * mMainWinCtx, JsonTree data = JsonTree())
        {
            ColorAdjustement::COUNT++;
            return ColorAdjustementRef( new ColorAdjustement( name, data, origin, vec2(WIDTH, 580), mMainWinCtx ) );
        }
        virtual JsonTree getData() override {
            {
                JsonTree obj = ModuleCommon::getData();
                obj.addChild(JsonTree("type", "ColorAdjustement"));
                JsonTree sub = JsonTree::makeObject("data");
                sub.addChild(data.bri.getData("bri", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("bri Limiter"))));
                sub.addChild(data.sat.getData("sat", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("sat Limiter"))));
                sub.addChild(data.con.getData("con", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("con Limiter"))));
                sub.addChild(data.red.getData("red", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("red Limiter"))));
                sub.addChild(data.green.getData("green", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("green Limiter"))));
                sub.addChild(data.blue.getData("blue", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("blue Limiter"))));
                sub.addChild(data.ssm.getData("ssm", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("ssm Limiter"))));
                sub.addChild(data.ssM.getData("ssM", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("ssM Limiter"))));
                obj.pushBack(sub);
                return obj;
            }
        };
        
        void setupShader() override;
        void setupUI() override;
        void setup() override;
        void update() override;
    };
    
    typedef std::shared_ptr<class ColorAdjustement> ColorAdjustementRef;
}


#endif /* ColorAdjustement_hpp */
