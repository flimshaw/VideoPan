#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class VideoPanApp : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
};

void VideoPanApp::setup()
{
}

void VideoPanApp::mouseDown( MouseEvent event )
{
}

void VideoPanApp::update()
{
}

void VideoPanApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}


CINDER_APP_BASIC( VideoPanApp, RendererGl )
