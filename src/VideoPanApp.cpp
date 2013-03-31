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
	int	mFrameRotation;
	
	CameraOrtho mCam;
	
	int mStartFrame;
	
	params::InterfaceGl mParams;
	
	FrameController mFrameController;
	
	float mFrameFocalDistance;
	float mFrameSpeed;
	int mFrameOffset;
	int mMaxFrames;
};

void VideoPanApp::prepareSettings( Settings *settings ){
    settings->setWindowSize( 1200, 850 );
    settings->setFrameRate( 30.0f );
}

void VideoPanApp::setup()
{
	mCam.setOrtho(-1, 1, -1, 1, -1, 1);
    
	try {
		fs::path path = getOpenFilePath( "" );
		if( ! path.empty() ) {
			mMoviePath = path.string();
		}
	}
	catch( ... ) {
		console() << "unable to load the texture file!" << std::endl;
	}
	
	mFrameFocalDistance = 500.0;
	mFrameSpeed = 5.0;
	mMaxFrames = 140;
	mStartFrame = 1;
	mFrameOffset = 0;
	mFrameRotation = 90;

	mParams = params::InterfaceGl( "Settings", Vec2i( 225, 200 ) );
	mParams.addParam( "Focal Distance", &mFrameFocalDistance, "min=1.0 max=15000.0 step=1 keyIncr=s keyDecr=w");
	mParams.addParam( "Camera Speed", &mFrameSpeed, "min=1.0 max=75.0 step=1 keyIncr=x keyDecr=z");
	mParams.addParam( "Max Frames", &mStartFrame, "min=0 max=80000 step=10 keyIncr=x keyDecr=z");
	mParams.addParam( "FrameRotation", &mFrameRotation, "min=0 max=359 step=1");
	mParams.addParam( "FrameOffset", &mFrameOffset, "min=-480 max=480 step=1");
	mFrameController = FrameController( mMoviePath, mStartFrame, mFrameOffset, mFrameSpeed, mFrameFocalDistance, mMaxFrames );
}

void VideoPanApp::update()
{
	mFrameController.setFrameFocalDistance( mFrameFocalDistance );
	mFrameController.setFrameSpeed( mFrameSpeed );
	mFrameController.setFrameOffset( mFrameOffset );
	mFrameController.setStartFrame( mStartFrame );
	mFrameController.update();
}

void VideoPanApp::draw()
{
	// clear out the window with black
	
	gl::clear( Color( 0, 0, 0 ) ); 
	
	gl::setMatricesWindow(getWindowSize(), true);

	gl::rotate(mFrameRotation);
	mFrameController.draw();
	params::InterfaceGl::draw();
}


CINDER_APP_BASIC( VideoPanApp, RendererGl )
