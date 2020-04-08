//
//  Osc.hpp
//  ORAGE
//
//  Created by Vincent Evrard on 3/04/20.
//

#ifndef Osc_hpp
#define Osc_hpp

#include "ModuleBase.h"
#include "OscListener.h"

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;


namespace ogre {
    class Osc : public ModuleCommon{
        
        gl::Context * mMainWinCtx;
        
        Osc(string name, JsonTree data, vec2 origin, vec2 size, gl::Context * mMainWinCtx);
        
    public:
        struct DATA {
            bool active = false;
            string address = "/p5";
            int port = 9001;
            float value = 0.0f;
            DATA(){}
            DATA(JsonTree data):
                active(data.getChild("active").getValue<bool>()),
                address(data.getChild("address").getValue<string>()),
                port(data.getChild("port").getValue<int>())
            {}
        };
        
        DATA data;
        TextInputRef address;
        TextInputRef port;
        
        static std::map<int, osc::Listener> LISTENERS;
        
        virtual ~Osc(){
            data.~DATA();
            mMainWinCtx = nullptr;
        }
        static int COUNT;
        
        typedef std::shared_ptr<class Osc> OscRef;
        
        static OscRef create( const std::string name, vec2 origin, gl::Context * mMainWinCtx, JsonTree data = JsonTree())
        {
            Osc::COUNT++;
            return OscRef( new Osc( name, data, origin, vec2(WIDTH*0.8, 280), mMainWinCtx ) );
        }
        
        virtual JsonTree getData() override {
            {
                JsonTree obj = ModuleCommon::getData();
                obj.addChild(JsonTree("type", "Osc"));
                JsonTree sub = JsonTree::makeObject("data");
                sub.addChild(JsonTree("active", data.active));
                sub.addChild(JsonTree("address", data.address));
                sub.addChild(JsonTree("port", data.port));
                obj.pushBack(sub);
                return obj;
            }
        };
        void setupUI() override;
        void setup() override;
        void update() override;
    };
    
    typedef std::shared_ptr<class Osc> OscRef;
}

#endif /* Lfos_hpp */

