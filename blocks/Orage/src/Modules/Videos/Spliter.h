//
//  Spliter.hpp
//  orage
//
//  Created by Vincent Evrard on 2017-04-20.
//
//

#ifndef Spliter_hpp
#define Spliter_hpp

#include "ModuleVideo.h"

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    class Spliter : public ModuleVideo{
        gl::Context * mMainWinCtx;
        
        gl::FboRef			mFbo;
        gl::GlslProgRef     mShader;
        
        Spliter(string name, JsonTree data, vec2 origin, vec2 size, gl::Context * mMainWinCtx);
    public:
        static int COUNT;
        
        virtual ~Spliter(){
            mFbo.reset();
            mShader.reset();
            mMainWinCtx = nullptr;
        }
        
        typedef std::shared_ptr<class Spliter> SpliterRef;
        
        static SpliterRef create( const std::string name, vec2 origin, gl::Context * mMainWinCtx, JsonTree data = JsonTree())
        {
            Spliter::COUNT++;
            return SpliterRef( new Spliter( name, data, origin, vec2(WIDTH, 580),  mMainWinCtx ) );
        }
        
        virtual JsonTree getData() override {
            {
                JsonTree obj = ModuleCommon::getData();
                obj.addChild(JsonTree("type", "Spliter"));
                return obj;
            }
        };
        void setupShader() override;
        void setupUI() override;
        void setup() override;
        void update() override;
    };
    
    typedef std::shared_ptr<class Spliter> SpliterRef;
}



#endif /* Spliter_hpp */
