//
//  Mosher.hpp
//  orage
//
//  Created by Vincent Evrard on 2017-04-20.
//
//

#ifndef Mosher_hpp
#define Mosher_hpp

#include "ModuleVideo.h"

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    class Mosher : public ModuleVideo{
        gl::Context * mMainWinCtx;
        
        struct DATA {
            float gainX  = .0f; //gain3
            float gainY  = .0f; //gain3
            float x     = .0f;
            float y     = .0f;
            bool mirror = false;
            DATA(){};
            DATA(JsonTree data):
            gainX(data.getChild("gainX").getValue<float>()),
            gainY(data.getChild("gainY").getValue<float>()),
            x(data.getChild("x").getValue<float>()),
                y(data.getChild("y").getValue<float>()),
                mirror(data.getChild("mirror").getValue<bool>())
            {}
        } ;
        DATA data;
        
        gl::UboRef          dataUbo;
        gl::FboRef			mFbo, mFbo2;
        gl::GlslProgRef     mShader;
        
        Mosher(string name, JsonTree data, vec2 origin, vec2 size, gl::Context * mMainWinCtx);
        
    public:
        static int COUNT;
        virtual ~Mosher(){
            data.~DATA();
            dataUbo.reset();
            mFbo.reset();
            mFbo2.reset();
            mShader.reset();
            mMainWinCtx = nullptr;
        }
        
        virtual void setData(int id, int elem, float nValue) override {
            switch(id){
                case 0 : data.gainX = lerp(-1.0f, 1.0f, nValue); break;
                case 1 : data.gainY = lerp(-1.0f, 1.0f, nValue); break;
                case 2 : data.x = lerp(-1.0f, 1.0f, nValue); break;
                case 3 : data.y = lerp(-1.f, 1.0f, nValue); break;
            }
        }
        
        typedef std::shared_ptr<class Mosher> MosherRef;
        
        static MosherRef create( const std::string name, vec2 origin, gl::Context * mMainWinCtx, JsonTree data = JsonTree())
        {
            Mosher::COUNT++;
            return MosherRef( new Mosher( name, data, origin, vec2(WIDTH, 580), mMainWinCtx ) );
        }
        
        
        virtual JsonTree getData() override {
            {
                JsonTree obj = ModuleCommon::getData();
                obj.addChild(JsonTree("type", "Mosher"));
                JsonTree sub = JsonTree::makeObject("data");
                sub.addChild(JsonTree("gainX", data.gainX));
                sub.addChild(JsonTree("gainY", data.gainY));
                sub.addChild(JsonTree("x", data.x));
                sub.addChild(JsonTree("y", data.y));
                sub.addChild(JsonTree("mirror", data.mirror));
                obj.pushBack(sub);
                return obj;
            }
        };
        void setupShader() override;
        void setupUI() override;
        void setup() override;
        void update() override;
    };
    
    typedef std::shared_ptr<class Mosher> MosherRef;
}


#endif /* Mosher_hpp */
