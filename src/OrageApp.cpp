#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "orage/all.h"

class OrageApp : public ci::app::App {
   std::vector<ModuleRef> modules;
   ModuleRef c;
  public:
	void setup() override;
   void mouseWheel(ci::app::MouseEvent event) override;
   void mouseMove(ci::app::MouseEvent event) override;
   void mouseDown(ci::app::MouseEvent event) override;
   void mouseUp(ci::app::MouseEvent event) override;
   void mouseDrag(ci::app::MouseEvent event) override;
	void update() override;
	void draw() override;
};

//////////////////////////////////

using namespace std;
using namespace ci;
using namespace ci::gl;
using namespace ci::app;

void OrageApp::setup()
{
   modules.push_back(Clock::create(60)->display(0, 0, 100, 100));
}

void OrageApp::update()
{
   for(auto module : modules){
      if(module->view != nullptr) module->view->update();
   }
}

void OrageApp::draw()
{
	clear( Color( 0, 0, 0 ) );
    
    pushModelView();
    translate( {0, 0} );
    for(auto module : modules){
      if(module->view != nullptr) module->view->draw();
    }
    popModelView();
}

void OrageApp::mouseWheel(MouseEvent event){

};

void OrageApp::mouseMove(MouseEvent event){
   //View::mousePos = event.getPos();
};

void OrageApp::mouseDown(MouseEvent event){
   //View::mouseLeftDown = event.isLeftDown();
};

void OrageApp::mouseUp(MouseEvent event){
   //View::mouseLeftDown = event.isLeftDown();
};

void OrageApp::mouseDrag(MouseEvent event){
   //View::mousePos = event.getPos();
};


CINDER_APP( OrageApp, RendererGl )
