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

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    class Oscillator : public ModuleVideo{
        gl::Context * mMainWinCtx;
        
        float trail = 1.0;
        
        struct DATA {
            float freq = 60.f;
            float sync = .0f;
            float exp = 1.f;
            float phaseDx = .0f;
            float phaseDy = .0f;
            float modifier = .0f;
            bool reverse = false;
            float sine = .0f;
            float rect = .0f;
            float saw = 1.f;
            float tri = .0f;
            float noise = .0f;
            DATA(){};
            DATA(JsonTree data):
                freq(data.getChild("freq").getValue<float>()),
                sync(data.getChild("sync").getValue<float>()),
                exp(data.getChild("exp").getValue<float>()),
                phaseDx(data.getChild("phaseDx").getValue<float>()),
                phaseDy(data.getChild("phaseDy").getValue<float>()),
                modifier(data.getChild("modifier").getValue<float>()),
                reverse(data.getChild("reverse").getValue<bool>()),
                sine(data.getChild("sine").getValue<float>()),
                rect(data.getChild("rect").getValue<float>()),
                tri(data.getChild("tri").getValue<float>()),
                noise(data.getChild("noise").getValue<float>())
            {}
        } ;
        DATA data;
        
        gl::UboRef          dataUbo;
        gl::FboRef			mFbo, mFbo2;
        gl::GlslProgRef     mShader;
        
        Oscillator(string name, JsonTree data, vec2 origin, vec2 size, gl::Context * mMainWinCtx);
        public:
        static int COUNT;
            virtual ~Oscillator(){
                cout<<"destroy osc"<<endl;
                data.~DATA();
                dataUbo.reset();
                mFbo.reset();
                mFbo2.reset();
                mShader.reset();
                mMainWinCtx = nullptr;
            }
        
            virtual void setData(int id, int elem, float nValue) override {
                switch(id){
                    case 0 : data.freq = lerp(0.125f, 600.0f, nValue); break;
                    case 1 : data.sync = lerp(0.0f, 1.0f, nValue); break;
                    case 2 : data.phaseDx = lerp(0.0f, 1.0f, nValue); break;
                    case 3 : data.phaseDy = lerp(-1.0f, 1.0f, nValue); break;
                    case 4 : data.modifier = lerp(.0f, 1.0f, nValue); break;
                    case 5 : trail = lerp(.0f, 1.0f, nValue); break;
                    case 6 : data.sine = lerp(-1.0f, 1.0f, nValue); break;
                    case 7 : data.rect = lerp(-1.0f, 1.0f, nValue); break;
                    case 8 : data.saw = lerp(-1.0f, 1.0f, nValue); break;
                    case 9 : data.tri = lerp(-1.0f, 1.0f, nValue); break;
                    case 10 : data.noise = lerp(-1.0f, 1.0f, nValue); break;
                    case 11 : data.exp = lerp(0.064f, 10.0f, nValue); break;
                }
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
                    sub.addChild(JsonTree("freq", data.freq));
                    sub.addChild(JsonTree("sync", data.sync));
                    sub.addChild(JsonTree("exp", data.exp));
                    sub.addChild(JsonTree("phaseDx", data.phaseDx));
                    sub.addChild(JsonTree("phaseDy", data.phaseDy));
                    sub.addChild(JsonTree("modifier", data.modifier));
                    sub.addChild(JsonTree("reverse", data.reverse));
                    sub.addChild(JsonTree("sine", data.sine));
                    sub.addChild(JsonTree("rect", data.rect));
                    sub.addChild(JsonTree("saw", data.saw));
                    sub.addChild(JsonTree("tri", data.tri));
                    sub.addChild(JsonTree("noise", data.noise));
                    sub.addChild(JsonTree("trail", trail));
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
