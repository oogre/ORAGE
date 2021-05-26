#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "core/Clock.h"
#include "core/LFO.h"

class BasicApp : public ci::app::App {
  	ClockRef clock;
  	LFORef lfo;
  	std::vector<ViewRef> views;
  	void update() override;
	void draw() override;
	void setup() override;
	void mouseWheel(ci::app::MouseEvent event) override;
	void mouseMove(ci::app::MouseEvent event) override;
    void mouseDown(ci::app::MouseEvent event) override;
    void mouseUp(ci::app::MouseEvent event) override;
    void mouseDrag(ci::app::MouseEvent event) override;
};

//////////////////////////////////////

using namespace std;
using namespace ci;
using namespace ci::app;

void prepareSettings( BasicApp::Settings* settings ){
	settings->setMultiTouchEnabled( false );
}

void BasicApp::setup(){
	clock = Clock::create(60);
	lfo = LFO::create();
	clock->strob([&](ClockEvent event) -> void{
		lfo->update(event);
	});

	views.push_back(clock->display(0, 0, 100, 100));
}
void BasicApp::mouseWheel(MouseEvent event){

}

void BasicApp::mouseMove(MouseEvent event){
	View::mousePos = event.getPos();
};

void BasicApp::mouseDown(MouseEvent event){
	View::mouseLeftDown = event.isLeftDown();
};

void BasicApp::mouseUp(MouseEvent event){
	View::mouseLeftDown = event.isLeftDown();
};

void BasicApp::mouseDrag(MouseEvent event){
	View::mousePos = event.getPos();
};

void BasicApp::update(){
	for(auto view : views){
		view->update();
	}
}

void BasicApp::draw(){
	gl::clear( Color::gray( 0.1f ) );

	for(auto view : views){
		view->draw();
	}
}

CINDER_APP( BasicApp, RendererGl, prepareSettings )
