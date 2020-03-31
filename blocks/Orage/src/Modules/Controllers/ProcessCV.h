//
//  ProcessCV.hpp
//  ORAGE2
//
//  Created by Vincent Evrard on 2017-06-30.
//
//

#ifndef ProcessCV_hpp
#define ProcessCV_hpp


#include "ModuleBase.h"

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    class ProcessCV : public ModuleCommon{
        
        gl::Context * mMainWinCtx;
        
        struct DATA {
            float master = 0.f;
            float reverse = 1.0f;
            DATA(){}
            DATA(JsonTree data):
                master(data.getChild("master").getValue<float>()),
                reverse(data.getChild("reverse").getValue<float>())
            {}
        };
        DATA data;
        
        ProcessCV(string name, JsonTree data, vec2 origin, vec2 size, gl::Context * mMainWinCtx);
    public:
        
        ~ProcessCV(){
            mMainWinCtx = nullptr;
        }
        static int COUNT;
        
        typedef std::shared_ptr<class ProcessCV> ProcessCVRef;
        
        static ProcessCVRef create( const std::string name, vec2 origin, gl::Context * mMainWinCtx, JsonTree data = JsonTree())
        {
            ProcessCV::COUNT++;
            return ProcessCVRef( new ProcessCV( name, data, origin, vec2(WIDTH*0.8, 280), mMainWinCtx ) );
        }
        
        virtual JsonTree getData() override {
            {
                JsonTree obj = ModuleCommon::getData();
                obj.addChild(JsonTree("module.type", "ProcessCV"));
                obj.addChild(JsonTree("data.master", data.master));
                obj.addChild(JsonTree("data.reverse", data.reverse));
                return obj;
            }
        };
        void setupUI() override;
        void setup() override;
        void update() override;
    };
    
    typedef std::shared_ptr<class ProcessCV> ProcessCVRef;
}


#endif /* ProcessCV_hpp */
