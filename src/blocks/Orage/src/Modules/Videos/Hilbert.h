//
//  Oscillator.hpp
//  orage
//
//  Created by Vincent Evrard on 2017-04-15.
//
//

#ifndef Hilbert_hpp
#define Hilbert_hpp

#include "ModuleVideo.h"

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    class Hilbert : public ModuleVideo{
        gl::Context * mMainWinCtx;
        
        float trail = 1.0;
        
        struct DATA {
            DATA(){};
            DATA(JsonTree data)
            {}
        } ;
        DATA data;
        
        gl::UboRef          dataUbo;
        gl::FboRef			mFbo, mFbo2;
        gl::GlslProgRef     mShader;
        
        Hilbert(string name, JsonTree data, vec2 origin, vec2 size, gl::Context * mMainWinCtx);
        public:
        static int COUNT;
            virtual ~Hilbert(){
                cout<<"destroy Hilbert"<<endl;
                data.~DATA();
                dataUbo.reset();
                mFbo.reset();
                mShader.reset();
                mMainWinCtx = nullptr;
            }
        
            virtual void setData(int id, int elem, float nValue) override {
                switch(id){
                }
            }
        
            typedef std::shared_ptr<class Hilbert> HilbertRef;
        
            static HilbertRef create( const std::string name, vec2 origin, gl::Context * mMainWinCtx, JsonTree data = JsonTree())
            {
                Hilbert::COUNT++;
                return HilbertRef( new Hilbert( name, data, origin, vec2(WIDTH, 580), mMainWinCtx ) );
            }
            virtual JsonTree getData() override {
                {
                    JsonTree obj = ModuleCommon::getData();
                    obj.addChild(JsonTree("type", "Hilbert"));
                    JsonTree sub = JsonTree::makeObject("data");
                    obj.pushBack(sub);
                    return obj;
                }
            };
            virtual void setupShader() override;
            virtual void setupUI() override;
            virtual void setup() override;
            virtual void update() override;
    };
    
    typedef std::shared_ptr<class Hilbert> HilbertRef;
}

#endif /* Hilbert_hpp */
