//
//  Freezer.hpp
//  PianoProjete
//
//  Created by Vincent Evrard on 2017-05-29.
//
//

#ifndef Freezer_hpp
#define Freezer_hpp


#include "ModuleVideo.h"

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    class Freezer : public ModuleVideo{
        gl::Context * mMainWinCtx;
        
        struct DATA {
            float amount  = 1.0f;
            float black  = 0.0f;
            float white  = 1.0f;
            bool selectorActive = false;
            DATA(){};
            DATA(JsonTree data):
                amount(data.getChild("amount").getValue<float>()),
                black(data.getChild("black").getValue<float>()),
                white(data.getChild("white").getValue<float>()),
                selectorActive(data.getChild("selectorActive").getValue<int>())
            {}
        };
        DATA data;
        
        gl::UboRef          dataUbo;
        gl::FboRef			mFbo, mFbo2;
        gl::GlslProgRef     mShader;
        
        Freezer(string name, JsonTree data, vec2 origin, vec2 size, gl::Context * mMainWinCtx);
        
    public:
        static int COUNT;
        virtual ~Freezer(){
            data.~DATA();
            dataUbo.reset();
            mFbo.reset();
            mFbo2.reset();
            mShader.reset();
            mMainWinCtx = nullptr;
        }
        
        typedef std::shared_ptr<class Freezer> FreezerRef;
        
        static FreezerRef create( const std::string name, vec2 origin, gl::Context * mMainWinCtx, JsonTree data = JsonTree())
        {
            Freezer::COUNT++;
            return FreezerRef( new Freezer( name, data, origin, vec2(WIDTH, 580), mMainWinCtx ) );
        }
        
        virtual JsonTree getData() override {
            {
                JsonTree obj = ModuleCommon::getData();
                obj.addChild(JsonTree("type", "Freezer"));
                JsonTree sub = JsonTree::makeObject("data");
                sub.addChild(JsonTree("amount", data.amount));
                sub.addChild(JsonTree("black", data.black));
                sub.addChild(JsonTree("white", data.white));
                sub.addChild(JsonTree("selectorActive", data.selectorActive));
                obj.pushBack(sub);
                return obj;
            }
        };
        void setupShader() override;
        void setupUI() override;
        void setup() override;
        void update() override;
    };
    
    typedef std::shared_ptr<class Freezer> FreezerRef;
}



#endif /* Freezer_hpp */
