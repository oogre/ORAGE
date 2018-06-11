#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

#include "ModuleController.hpp"

#include "InteractionManager.hpp"

using namespace ci;
using namespace ci::app;
using namespace std;

class OrageApp : public App {
    ModuleController moduleCnt;
   
  public:
    static void prepareSettings( Settings *settings );
	void setup() override;
    void update() override;
	void draw() override;
    ivec2 mMouseLocation;
    bool mMouseDown = false;
};

void OrageApp::setup(){
    InteractionManager::init();
}

void OrageApp::update(){
    moduleCnt.update();
    
}

void OrageApp::draw(){
	gl::clear( Color( 0, 0, 0 ) );
    moduleCnt.draw();
}

void OrageApp::prepareSettings( Settings *settings ){
    settings->setWindowSize( 1200, 600 );//1920, 1080 );
    //settings->setWindowPos(0, 0);
    // settings->setFullScreen(true, FullScreenOptions().display(FullScreenOptions().getDisplay()));
}

CINDER_APP( OrageApp, RendererGl, &OrageApp::prepareSettings )
