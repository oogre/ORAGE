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
    typedef ORAGE::COMMON::Event<ORAGE::CORE::Module> ModuleEvt;
    typedef ORAGE::COMMON::Event<ORAGE::CORE::Cable> CableCoreEvt;
    typedef ORAGE::COMMON::Event<ORAGE::UI::View> ViewEvt;
    typedef ORAGE::COMMON::Event<ORAGE::UI::Cable> CableUiEvt;
    typedef ORAGE::CORE::ModuleType ModuleType;
    typedef ORAGE::UI::ViewType ViewType;
    
    CORE::Instance().module->addEventListener("add", [&](ModuleEvt event) -> void{
        UI::Instance().addView(event.target);
    });
    CORE::Instance().cables->addEventListener("add", [&](CableCoreEvt event) -> void{
        UI::Instance().addCable( event.target->input, event.target->output, false);
    });
    UI::Instance().view->addEventListener("add", [&](ViewEvt event) -> void{
        //cout<<event.target->getName(true)<<endl;
    });
    UI::Instance().cables->addEventListener("add", [&](CableUiEvt event) -> void{
        ORAGE::CORE::ModuleRef input = event.target->input->getParent<ORAGE::UI::View>([&](ORAGE::UI::ViewRef view)->bool{
            return view->getModule()!= nullptr;
        })->getModule();
        ORAGE::CORE::ModuleRef output = event.target->output->getParent<ORAGE::UI::View>([&](ORAGE::UI::ViewRef view)->bool{
            return view->getModule()!= nullptr;
        })->getModule();
        CORE::Instance().addCable(input, output, false);
    });
    
    CORE::Instance().addModules(json::array({
        json::object({
            {"name", "wrapper"},
            {"type", ModuleType::Module},
            {"view", ViewType::Pannel},
            {"submodules", json::array({
                json::object({
                    {"name", "numberI"},
                    {"type", ModuleType::NumberI},
                    {"view", ViewType::Number},
                    {"value", 60},
                    {"display", true},
                    {"position", {
                        {"x", 20},
                        {"y", 100},
                    }}
                }),
                json::object({
                    {"name", "numberII"},
                    {"type", ModuleType::NumberI},
                    {"view", ViewType::Number},
                    {"value", 180},
                    {"display", true},
                    {"position", {
                        {"x", 180},
                        {"y", 100},
                    }}
                }),
                json::object({
                    {"name", "bang"},
                    {"type", ModuleType::NumberI},
                    {"view", ViewType::BangButton},
                    {"on", 1},
                    {"off", 0},
                    {"value", 0},
                    {"position", {
                        {"x", 280},
                        {"y", 100},
                    }}
                }),
                json::object({
                    {"name", "toggle"},
                    {"type", ModuleType::NumberI},
                    {"view", ViewType::ToggleButton},
                    {"on", 1},
                    {"off", 0},
                    {"value", 0},
                    {"position", {
                        {"x", 380},
                        {"y", 100},
                    }}
                }),
            })},
            {"position", {
                {"x", 20},
                {"y", 100},
            }}
        }),
        json::object({
            {"name", "numberI"},
            {"type", ModuleType::NumberI},
            {"view", ViewType::Number},
            {"value", 60},
            {"display", true},
            {"position", {
                {"x", 200},
                {"y", 100},
            }}
        }),
    }));
    
    cout<<CORE::Instance().to_string()<<endl;
    cout<<UI::Instance().to_string()<<endl;
    
//    CORE::Instance().addCables(json::array({
//        json::array({"numberI", "numberII"})
//    }));
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
