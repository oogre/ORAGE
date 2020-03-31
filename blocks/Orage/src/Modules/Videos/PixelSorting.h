//
//  PixelSorting.hpp
//  PianoProjete
//
//  Created by Vincent Evrard on 2017-06-19.
//
//

#ifndef PixelSorting_hpp
#define PixelSorting_hpp


#include "ModuleVideo.h"

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    class PixelSorting : public ModuleVideo{
        gl::Context * mMainWinCtx;
        
        struct DATA {
            float start = 0.0f;
            float stop  = 1.0f;
            int width = FBO_WIDTH;
            int height  = FBO_HEIGHT;
            DATA(){};
            DATA(JsonTree data):
                start(data.getChild("start").getValue<float>()),
                stop(data.getChild("stop").getValue<float>())
            {};
        };
        DATA data;
        
        gl::UboRef          dataUbo;
        gl::FboRef			mFbo, mFbo2;
        gl::GlslProgRef     mShader;
        
        PixelSorting(string name, JsonTree jsonData, vec2 origin, vec2 size, gl::Context * mMainWinCtx);
        
    public:
        static int COUNT;
        ~PixelSorting(){
            data.~DATA();
            dataUbo.reset();
            mFbo.reset();
            mFbo2.reset();
            mShader.reset();
            mMainWinCtx = nullptr;
        }
        
        typedef std::shared_ptr<class PixelSorting> PixelSortingRef;
        
        static PixelSortingRef create( const std::string name, vec2 origin, gl::Context * mMainWinCtx, JsonTree data = JsonTree())
        {
            PixelSorting::COUNT++;
            return PixelSortingRef( new PixelSorting( name, data, origin, vec2(WIDTH, 580), mMainWinCtx ) );
        }
        
        virtual JsonTree getData() override {
            {
                JsonTree obj = ModuleCommon::getData();
                obj.addChild(JsonTree("type", "PixelSorting"));
                JsonTree sub = JsonTree::makeObject("data");
                sub.addChild(JsonTree("start", data.start));
                sub.addChild(JsonTree("stop", data.stop));
                obj.pushBack(sub);
                return obj;
            }
        };
        void setupShader() override;
        void setupUI() override;
        void setup() override;
        void update() override;
    };
    
    typedef std::shared_ptr<class PixelSorting> PixelSortingRef;
}


#endif /* PixelSorting_hpp */
