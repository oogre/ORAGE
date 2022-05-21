//
//  Lfos.hpp
//  orage
//
//  Created by Vincent Evrard on 2017-04-22.
//
//

#ifndef Passthrough_hpp
#define Passthrough_hpp

#include "ModuleBase.h"
#include "cinder/Perlin.h"

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    
    class Passthrough : public ModuleCommon{
        
        gl::Context * mMainWinCtx;
        Passthrough(string name, JsonTree data, vec2 origin, vec2 size, gl::Context * mMainWinCtx);
        
    public:
        
        struct DATA {
            float pass;
            float value;
            float output;
            DATA():
                pass(0.0f),
                value(0.0f),
                output(0.0f)
            {}
            DATA(JsonTree data):
                pass(data.getChild("pass").getValue<float>()),
                value(data.getChild("value").getValue<float>()),
                output(data.getChild("output").getValue<float>())
            {}
        };
        
        DATA data;
        
        virtual ~Passthrough(){
            data.~DATA();
            mMainWinCtx = nullptr;
        }
        static int COUNT;
        
        typedef std::shared_ptr<class Passthrough> PassthroughRef;
        
        static PassthroughRef create( const std::string name, vec2 origin, gl::Context * mMainWinCtx, JsonTree data = JsonTree())
        {
            Passthrough::COUNT++;
            return PassthroughRef( new Passthrough( name, data, origin, vec2(WIDTH*0.8, 280), mMainWinCtx ) );
        }
        
        virtual JsonTree getData() override {
            {
                JsonTree obj = ModuleCommon::getData();
                obj.addChild(JsonTree("type", "Passthrough"));
                JsonTree sub = JsonTree::makeObject("data");
                sub.addChild(JsonTree("pass", data.pass));
                sub.addChild(JsonTree("value", data.value));
                sub.addChild(JsonTree("output", data.output));
                obj.pushBack(sub);
                return obj;
            }
        };
        void setupUI() override;
        void setup() override;
        void update() override;
    };
    
    typedef std::shared_ptr<class Passthrough> PassthroughRef;
}

#endif /* Passthrough_hpp */
