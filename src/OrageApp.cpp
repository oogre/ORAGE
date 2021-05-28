#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include <math.h>       /* modf */
#include "orage/all.h"

class OrageApp : public ci::app::App {
    std::vector<ModuleRef> modules;
    ModuleRef c;
    float b = 60;
    public:
        void setup() override;
        void update() override;
        void draw() override;
};



//////////////////////////////////

using namespace std;
using namespace ci;
using namespace ci::gl;
using namespace ci::app;

void OrageApp::setup(){
   modules.push_back(Clock::create(60)->display(100, 100, 100, 100));
}

void OrageApp::update(){
    b+=1;
    if(b > 300){
        b = 0;
    }
    dynamic_pointer_cast<Clock>(modules[0])->setBPM(b);
    for(auto module : modules){
        if(module->view != nullptr) module->view->update();
    }
}

void OrageApp::draw(){
	clear( Color( 0, 0, 0 ) );
    pushModelView();
    translate( {0, 0} );
    for(auto module : modules){
      if(module->view != nullptr) module->view->draw();
    }
    popModelView();
}


CINDER_APP( OrageApp, RendererGl )
