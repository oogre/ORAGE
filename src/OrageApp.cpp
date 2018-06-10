#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

#include "ModuleController.hpp"
#include "UIController.hpp"
#include "InteractionManager.hpp"
#include "Slider.hpp"
using namespace ci;
using namespace ci::app;
using namespace std;

class OrageApp : public App {
    ModuleController moduleCnt;
    UIController uiCnt;
    Slider sliderA = Slider("SLider A", cinder::ivec2(0, 100));
    Slider sliderB = Slider("SLider B", cinder::ivec2(0, 200));
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
    uiCnt.update();
    sliderA.update();
    sliderB.update();
}

void OrageApp::draw(){
	gl::clear( Color( 0, 0, 0 ) );
    moduleCnt.draw();
    uiCnt.draw();
    sliderA.draw();
    sliderB.draw();
}

void OrageApp::prepareSettings( Settings *settings ){
    settings->setWindowSize( 1200, 600 );//1920, 1080 );
    //settings->setWindowPos(0, 0);
    // settings->setFullScreen(true, FullScreenOptions().display(FullScreenOptions().getDisplay()));
}

CINDER_APP( OrageApp, RendererGl, &OrageApp::prepareSettings )
