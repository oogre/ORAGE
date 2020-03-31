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
        
        Random(string name, vec2 origin, vec2 size, gl::Context * mMainWinCtx);
        
    public:
        float acc = 0.0f;
        float oldInput = 0.0f;
        struct RandomData {
            float input = .0f;
            float linear = .0f;
        }randomDatas;
        Perlin p;
        virtual ~Random(){
            randomDatas.~RandomData();
            mMainWinCtx = nullptr;
        }
        static int COUNT;
        
        typedef std::shared_ptr<class Random> RandomRef;
        
        static RandomRef create( const std::string name, vec2 origin, gl::Context * mMainWinCtx)
        {
            Random::COUNT++;
            return RandomRef( new Random( name, origin, vec2(WIDTH*0.8, 280), mMainWinCtx ) );
        }
        virtual JsonTree getData() override {
            {
                JsonTree obj = ModuleCommon::getData();
                obj.addChild(JsonTree("module.type", "Random"));
                obj.addChild(JsonTree("data.input", randomDatas.input));
                obj.addChild(JsonTree("data.linear", randomDatas.linear));
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

