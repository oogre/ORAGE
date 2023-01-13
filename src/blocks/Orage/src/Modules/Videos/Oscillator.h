//
//  Oscillator.hpp
//  orage
//
//  Created by Vincent Evrard on 2017-04-15.
//
//

#ifndef Oscillator_hpp
#define Oscillator_hpp

#include "ModuleVideo.h"
#include "DataRange.h"

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    class Oscillator : public ModuleVideo{
        gl::Context * mMainWinCtx;
        
        struct DATA {
            RANGE freq;
            RANGE sync;
            RANGE exp;
            RANGE phaseDx;
            RANGE phaseDy;
            RANGE modifier;
            RANGE sine;
            RANGE rect;
            RANGE saw;
            RANGE tri;
            RANGE noise;
            RANGE trail;
            DATA() :
                freq(60.0f, 0.0125f, 600.0f),
                sync(.0f, 0.0f, 1.0f),
                exp(1.0f, 0.064f, 10.0f),
                phaseDx(.0f, 0.0f, 1.0f),
                phaseDy(.0f, -1.0f, 1.0f),
                modifier(.0f, 0.0f, 1.0f),
                sine(.0f, -1.0f, 1.0f),
                rect(.0f, -1.0f, 1.0f),
                saw(1.0f, -1.0f, 1.0f),
                tri(.0f, -1.0f, 1.0f),
                noise(.0f, -1.0f, 1.0f),
                trail(1.0f, 0.0f, 1.0f)
            {};
            DATA(JsonTree data):
                freq(data.getChild("freq")),
                sync(data.getChild("sync")),
                exp(data.getChild("exp")),
                phaseDx(data.getChild("phaseDx")),
                phaseDy(data.getChild("phaseDy")),
                modifier(data.getChild("modifier")),
                sine(data.getChild("sine")),
                rect(data.getChild("rect")),
                saw(data.getChild("saw")),
                tri(data.getChild("tri")),
                noise(data.getChild("noise")),
                trail(data.getChild("trail"))
            {}
        } ;
        DATA data;
        
        struct S_DATA {
            float freq;
            float sync;
            float exp;
            float phaseDx;
            float phaseDy;
            float modifier;
            float sine;
            float rect;
            float saw;
            float tri;
            float noise;
        } ;
        S_DATA sData;
        
        gl::UboRef          dataUbo;
        gl::FboRef			mFbo, mFbo2;
        gl::GlslProgRef     mShader;
        
        Oscillator(string name, JsonTree data, vec2 origin, vec2 size, gl::Context * mMainWinCtx);
        public:
        static int COUNT;
            virtual ~Oscillator(){
                data.~DATA();
                dataUbo.reset();
                mFbo.reset();
                mFbo2.reset();
                mShader.reset();
                mMainWinCtx = nullptr;
            }
        
            typedef std::shared_ptr<class Oscillator> OscillatorRef;
        
            static OscillatorRef create( const std::string name, vec2 origin, gl::Context * mMainWinCtx, JsonTree data = JsonTree())
            {
                Oscillator::COUNT++;
                return OscillatorRef( new Oscillator( name, data, origin, vec2(WIDTH, 580), mMainWinCtx ) );
            }
            virtual JsonTree getData() override {
                {
                    JsonTree obj = ModuleCommon::getData();
                    obj.addChild(JsonTree("type", "Oscillator"));
                    JsonTree sub = JsonTree::makeObject("data");
                    sub.addChild(data.freq.getData("freq", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("Hz Limiter"))));
                    sub.addChild(data.sync.getData("sync", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("Sync Limiter"))));
                    sub.addChild(data.exp.getData("exp", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("Exp Limiter"))));
                    sub.addChild(data.phaseDx.getData("phaseDx", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("Dx Limiter"))));
                    sub.addChild(data.phaseDy.getData("phaseDy", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("Dy Limiter"))));
                    sub.addChild(data.modifier.getData("modifier", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("Mod Limiter"))));
                    sub.addChild(data.sine.getData("sine", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("Sine Limiter"))));
                    sub.addChild(data.rect.getData("rect", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("Rect Limiter"))));
                    sub.addChild(data.saw.getData("saw", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("Saw Limiter"))));
                    sub.addChild(data.tri.getData("tri", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("Tri Limiter"))));
                    sub.addChild(data.noise.getData("noise", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("Noise Limiter"))));
                    sub.addChild(data.trail.getData("trail", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("Trail Limiter"))));
                    obj.pushBack(sub);
                    return obj;
                }
            };
       
        
        
            virtual void setupShader() override;
            virtual void setupUI() override;
            virtual void setup() override;
            virtual void update() override;
    };
    
    typedef std::shared_ptr<class Oscillator> OscillatorRef;
}

#endif /* Oscillator_hpp */
