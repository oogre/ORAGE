#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include <math.h>       /* modf */
//#include "orage/all.h"
#include "lib/json.hpp"
#include "orage/core/Core.h"
#include "orage/UI/UI.h"

class OrageApp : public ci::app::App {
    //ModuleRef orage;
    
    public:
        void setup() override;
        void update() override;
        void draw() override;
//        void mouseUp(ci::app::MouseEvent e) override {
//            cout<<ORAGE::CORE::Manager::Instance().getModule("bang")->getConf().dump()<<endl;
//        }
};

//////////////////////////////////

using namespace std;
using namespace ci;
using namespace ci::gl;
using namespace ci::app;

using json = nlohmann::json;
using CORE = ORAGE::CORE::Manager;
using UI = ORAGE::UI::Manager;

void OrageApp::setup(){
    CORE::Instance().addEventListener("add", [&](ORAGE::CORE::ParameterEvent event) -> void{
        UI::Instance().addView(event.target);
        
        //cout<<event.target->as<ORAGE::CORE::Parameter>()->getName()<<endl;
    });

//    CORE::Instance().addModule({
//        {"name", "clock"},
//        {"type", ORAGE::CORE::ModuleType::Clock},
//        {"subModule" , json::array({
//            json::object({
//                {"name", "active"},
//                {"type", ORAGE::CORE::ModuleType::NumberI},
//                {"value", 1}
//            }),
//            json::object({
//                {"name", "bpm"},
//                {"type", ORAGE::CORE::ModuleType::NumberI},
//                {"value", 130}
//            }),
//            json::object({
//                {"name", "bang"},
//                {"type", ORAGE::CORE::ModuleType::NumberI},
//                {"value", 1}
//            })
//        })},
//        {"position", {
//            {"x", 100},
//            {"y", 100},
//        }}
//    });
    CORE::Instance().addModule({
        {"name", "numberI"},
        {"type", ORAGE::CORE::ModuleType::NumberI},
        {"view", ORAGE::UI::ViewType::Number},
        {"value", 60},
        {"defaultValue", 0},
        {"position", {
            {"x", 20},
            {"y", 100},
        }}
    });
    
    CORE::Instance().addModule({
        {"name", "numberF"},
        {"type", ORAGE::CORE::ModuleType::NumberF},
        {"view", ORAGE::UI::ViewType::Number},
        {"value", 3.14},
        {"defaultValue", 0},
        {"position", {
            {"x", 90},
            {"y", 100},
        }}
    });
    
    CORE::Instance().addModule({
        {"name", "bang"},
        {"type", ORAGE::CORE::ModuleType::NumberI},
        {"view", ORAGE::UI::ViewType::BangButton},
        {"on", 1},
        {"off", 0},
        {"value", 0},
        {"defaultValue", 0},
        {"position", {
            {"x", 160},
            {"y", 100},
        }}
    });
    
    CORE::Instance().addModule({
        {"name", "toggle"},
        {"type", ORAGE::CORE::ModuleType::NumberI},
        {"view", ORAGE::UI::ViewType::ToggleButton},
        {"on", 1},
        {"off", 0},
        {"value", 1},
        {"defaultValue", 0},
        {"position", {
            {"x", 200},
            {"y", 100},
        }}
    });
    
    CORE::Instance().addCables(json::array({
        json::array({"numberF", "numberI"}),
        json::array({"numberI", "toggle"}),
    }));

    cout << CORE::Instance().to_string() << endl;
    cout << UI::Instance().to_string() << endl;
}

void OrageApp::update(){
    UI::Manager::Instance().update();
}

void OrageApp::draw(){
    UI::Manager::Instance().draw();
}


CINDER_APP( OrageApp, RendererGl, []( App::Settings *settings ) {
    vector<DisplayRef> displays = Display::getDisplays();
    Rectf mainDisplayBounds;

    for(auto display : displays){
        auto displaySize = display->getBounds();
        
        if(displaySize.getX1() == 0 && displaySize.getY1() == 0){
            mainDisplayBounds = displaySize;
            settings->setDisplay(display);
        }
    }
    cout<<mainDisplayBounds<<endl;
    settings->setWindowSize(mainDisplayBounds.getWidth()/3, mainDisplayBounds.getHeight()/3);
    settings->setWindowPos(
        (mainDisplayBounds.getWidth() - mainDisplayBounds.getWidth()/3)/2,
        (mainDisplayBounds.getHeight()- mainDisplayBounds.getHeight()/3)/2);
    settings->setTitle("ORAGE");
} )
