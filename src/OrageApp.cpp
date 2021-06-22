#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include <math.h>       /* modf */
#include "orage/all.h"

class OrageApp : public ci::app::App {
    ModuleRef orage;
    public:
        void setup() override;
        void update() override;
        void draw() override;
        void mouseDown(ci::app::MouseEvent event) override;
};

//////////////////////////////////

using namespace std;
using namespace ci;
using namespace ci::gl;
using namespace ci::app;

void OrageApp::mouseDown(MouseEvent event) {
//    ModuleRef clock = orage->getSubModule("clock");
//    if(clock->pannel == nullptr){
//        clock->display();
//    } else {
//        clock->hide();
//    }
}

void OrageApp::setup(){
    orage = Module::create();
    orage->setName("orage");
    orage->display(0, 0, 800, 600);
    orage->addSubModule<Clock>("clock", Clock::create(60))->display(100, 100, 130, 60);
    orage->addSubModule<LFO>("lfo·1", LFO::create())->display(250, 100, 130, 215);
    orage->addSubModule<LFO>("lfo·2", LFO::create())->display(400, 100, 130, 215);
    
    orage->getSubModule<Clock>("clock")->getClockSignal()->connect(boost::bind(&LFO::run, orage->getSubModule<LFO>("lfo·1"), _1));
    orage->getSubModule<Clock>("clock")->getClockSignal()->connect(boost::bind(&LFO::run, orage->getSubModule<LFO>("lfo·2"), _1));
    
    orage->getSubModule<Module>("lfo·1")->getSubModule<ParameterI>("div")->addSlave(
                                                                                    orage->getSubModule<Module>("lfo·2")->getSubModule<ParameterI>("div"));
}

void OrageApp::update(){
    orage->update();
}

void OrageApp::draw(){
	clear( Color( 0, 0, 0 ) );
    orage->draw();
}


CINDER_APP( OrageApp, RendererGl )
