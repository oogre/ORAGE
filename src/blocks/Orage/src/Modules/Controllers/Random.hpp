//
//  Lfos.hpp
//  orage
//
//  Created by Vincent Evrard on 2017-04-22.
//
//

#ifndef Random_hpp
#define Random_hpp

#include "ModuleBase.h"
#include "cinder/Perlin.h"

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    class Random : public ModuleCommon{
        
        gl::Context * mMainWinCtx;
        
        Random(string name, JsonTree data, vec2 origin, vec2 size, gl::Context * mMainWinCtx);
        
    public:
        float acc = 0.0f;
        float oldInput = 0.0f;
        
        
        
        struct DATA {
            float input;
            float oldInput;
            float counter;
            float linear;
            float randomness;
            float seed = random();
            DATA():
                input(0.0f),
                oldInput(0.0f),
                counter(0.0f),
                linear(0.0f),
                randomness(0.5f),
                seed(random())
            {}
            DATA(JsonTree data):
                input(data.getChild("input").getValue<float>()),
                oldInput(data.getChild("oldInput").getValue<float>()),
                counter(data.getChild("counter").getValue<float>()),
                linear(data.getChild("linear").getValue<float>()),
                randomness(data.getChild("randomness").getValue<float>()),
                seed(data.getChild("seed").getValue<float>())
            {}
        };
        DATA data;
        
        Perlin p;
        virtual ~Random(){
            data.~DATA();
            mMainWinCtx = nullptr;
        }
        static int COUNT;
        
        typedef std::shared_ptr<class Random> RandomRef;
        
        static RandomRef create( const std::string name, vec2 origin, gl::Context * mMainWinCtx, JsonTree data = JsonTree())
        {
            Random::COUNT++;
            return RandomRef( new Random( name, data, origin, vec2(WIDTH*0.8, 280), mMainWinCtx ) );
        }
        virtual JsonTree getData() override {
            {
                JsonTree obj = ModuleCommon::getData();
                obj.addChild(JsonTree("type", "Random"));
                JsonTree sub = JsonTree::makeObject("data");
                sub.addChild(JsonTree("input", data.input));
                sub.addChild(JsonTree("oldInput", data.oldInput));
                sub.addChild(JsonTree("counter", data.counter));
                sub.addChild(JsonTree("linear", data.linear));
                sub.addChild(JsonTree("randomness", data.randomness));
                sub.addChild(JsonTree("seed", data.seed));
                obj.pushBack(sub);
                return obj;
            }
        };
        void setupUI() override;
        void setup() override;
        void update() override;
    };
    
    typedef std::shared_ptr<class Random> RandomRef;
}

#endif /* Random_hpp */

