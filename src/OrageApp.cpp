#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "Pannel.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class OrageApp : public App {
    PannelRef pannel;
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

//////////////////////////////////

using namespace std;
using namespace ci;
using namespace ci::gl;



void OrageApp::setup()
{
    pannel = Pannel::create({100, 100}, {100, 100});
}

void OrageApp::mouseDown( MouseEvent event )
{
}

void OrageApp::update()
{
    pannel->update();
}

void OrageApp::draw()
{
	clear( Color( 0, 0, 0 ) );
    pushModelView();
    translate( {0, 0} );
    pannel->draw();
    popModelView();
}

CINDER_APP( OrageApp, RendererGl )
