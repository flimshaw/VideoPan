#include "cinder/app/AppBasic.h"
#include "cinder/ImageIO.h"
#include "cinder/gl/Texture.h"
#include "cinder/qtime/QuickTime.h"
#include "cinder/Utilities.h"
#include "FrameController.h"
#include "cinder/Camera.h"
#include "cinder/params/Params.h"


using namespace ci;
using namespace ci::app;
using namespace std;

class VideoPanApp : public AppBasic {
  public:
	void setup();
	void prepareSettings( Settings *settings );
	void loadMovieFile( const std::string &path );
	void update();
	void draw();
	
	gl::Texture mMovieFrame;
	qtime::MovieGl mMovie;
	string mMoviePath;
	
	CameraOrtho mCam;
	
	int mStartFrame;
	
	params::InterfaceGl mParams;
	
	FrameController mFrameController;
	
	float mFrameFocalDistance;
	float mFrameSpeed;
	float mFrameOffset;
	int mMaxFrames;
};

void VideoPanApp::prepareSettings( Settings *settings ){
    settings->setWindowSize( 800, 960 );
    settings->setFrameRate( 30.0f );
}

void VideoPanApp::setup()
{
	mCam.setOrtho(-1, 1, -1, 1, -1, 1);
	
	mMoviePath = "/Volumes/SPEEDY/desert_right.mov";
	
	mFrameFocalDistance = 10.0;
	mFrameSpeed = 60.0;
	mMaxFrames = 10;
	mFrameOffset = 0;

	mParams = params::InterfaceGl( "Settings", Vec2i( 225, 200 ) );
	mParams.addParam( "Focal Distance", &mFrameFocalDistance, "min=1.0 max=100.0 step=.1 keyIncr=s keyDecr=w");
	mParams.addParam( "Camera Speed", &mFrameSpeed, "min=20.0 max=75.0 step=1 keyIncr=x keyDecr=z");
	mParams.addParam( "Max Frames", &mMaxFrames, "min=10 max=50 step=1 keyIncr=x keyDecr=z");
	
	mFrameController = FrameController( mMoviePath, mStartFrame, mFrameOffset, mFrameSpeed, mFrameFocalDistance, mMaxFrames );
}

void VideoPanApp::update()
{
	mFrameController.update();
}

void VideoPanApp::draw()
{
	// clear out the window with black
	
	gl::clear( Color( 0, 0, 0 ) ); 
	
	gl::setMatricesWindow(getWindowSize(), true);

	gl::rotate(90);
	mFrameController.draw();
	params::InterfaceGl::draw();
}


CINDER_APP_BASIC( VideoPanApp, RendererGl )
