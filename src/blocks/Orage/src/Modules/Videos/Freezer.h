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
#include "DataRange.h"

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    class Freezer : public ModuleVideo{
        gl::Context * mMainWinCtx;
        
        struct DATA {
            RANGE amount;
            RANGE black;
            RANGE white;
            DATA() :
                amount(1.0f, 0.0f, 1.0f),
                black(0.0f, 0.0f, 1.0f),
                white(1.0f, 0.0f, 1.0f)
            {}
            DATA(JsonTree data):
                amount(data.getChild("amount")),
                black(data.getChild("black")),
                white(data.getChild("white"))
            {}
        };
        DATA data;
        
        struct S_DATA {
            float amount;
            float black;
            float white;
            int selectorActive;
        };
        S_DATA sData;
        
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
                sub.addChild(data.amount.getData("amount", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("amount Limiter"))));
                sub.addChild(data.black.getData("black", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("black Limiter"))));
                sub.addChild(data.white.getData("white", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("white Limiter"))));
                
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
