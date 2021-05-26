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

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    class TintCorrector : public ModuleVideo{
        gl::Context * mMainWinCtx;
        
        struct DATA {
            float hue  = 0.0f;
            float sat  = 0.0f;
            float light  = 0.0f;
            int modifier = 0;
            DATA(){};
            DATA(JsonTree data):
                hue(data.getChild("hue").getValue<float>()),
                sat(data.getChild("sat").getValue<float>()),
                light(data.getChild("light").getValue<float>()),
                modifier(data.getChild("modifier").getValue<int>())
            {}
        } ;
        DATA data;
        
        
        
        gl::UboRef          dataUbo;
        gl::FboRef			mFbo, mFbo2;
        gl::GlslProgRef     mShader;
        
        TintCorrector(string name, JsonTree data, vec2 origin, vec2 size, gl::Context * mMainWinCtx);
        
    public:
        static int COUNT;
        
        virtual ~TintCorrector(){
            data.~DATA();
            dataUbo.reset();
            mFbo.reset();
            mShader.reset();
            mMainWinCtx = nullptr;
        }
        
        virtual void setData(int id, int elem, float nValue) override {
            switch(id){
                case 0 : data.hue = lerp(-1.0f, 1.0f, nValue); break;
                case 1 : data.sat = lerp(-1.0f, 1.0f, nValue); break;
                case 2 : data.light = lerp(-1.0f, 1.0f, nValue); break;
            }
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
                sub.addChild(JsonTree("hue", data.hue));
                sub.addChild(JsonTree("sat", data.sat));
                sub.addChild(JsonTree("light", data.light));
                sub.addChild(JsonTree("modifier", data.modifier));
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
