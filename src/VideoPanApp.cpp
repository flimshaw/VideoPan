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
	void loopOffset();
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
	float mFrameOffset;
	float mCameraPosition;
	float mOffsetTicker;
	float mOffsetVelocity;
	
	Vec3f mEye;
	Vec3f mCenter;
	Vec3f mUp;

	int mMaxFrames;
};

void VideoPanApp::prepareSettings( Settings *settings ){
    settings->setWindowSize( 1200, 850 );
    settings->setFrameRate( 30.0f );
}

void VideoPanApp::setup()
{
	mCam.setOrtho(-1, 1, -1, 1, -1, 1);
	
	mEye        = Vec3f( 0.0f, 0.0f, 0.0f );
	mCenter     = Vec3f::zero();
	mUp         = Vec3f::yAxis();
	
	//mMoviePath = "/Users/choey/valiant_files/2011-07-30/UP/video_002.mp4";
	mMoviePath = "/Users/choey/tower-flythrough-desktop.m4v";
	
	mFrameFocalDistance = 500.0;
	mFrameSpeed = 5.0;
	mMaxFrames = 300;
	mStartFrame = 1;
	mFrameOffset = 0;
	mFrameRotation = 90;
	mCameraPosition = 0.0;
	
	mOffsetVelocity = 1.0;
	mOffsetTicker = 0;

	mParams = params::InterfaceGl( "Settings", Vec2i( 225, 200 ) );
	mParams.addParam( "Focal Distance", &mFrameFocalDistance, "min=1.0 max=15000.0 step=1 keyIncr=s keyDecr=w");
	mParams.addParam( "Camera Speed", &mFrameSpeed, "min=1.0 max=75.0 step=1 keyIncr=x keyDecr=z");
	mParams.addParam( "Jump to Frame", &mStartFrame, "min=0 max=80000 step=10 keyIncr=x keyDecr=z");
	mParams.addParam( "Offset Velocity", &mOffsetVelocity, "min=-100.0 max=100.0 step=.1 keyIncr=x keyDecr=z");
	mParams.addParam( "FrameRotation", &mFrameRotation, "min=0 max=359 step=1");
	mParams.addParam( "FrameOffset", &mFrameOffset, "min=-480 max=480 step=1");
	mFrameController = FrameController( mMoviePath, mStartFrame, mFrameOffset, mFrameSpeed, mFrameFocalDistance, mMaxFrames );
}

void VideoPanApp::update()
{
	loopOffset();
	mFrameController.setFrameFocalDistance( mFrameFocalDistance );
	mFrameController.setFrameSpeed( mFrameSpeed );
	mFrameController.setFrameOffset( mOffsetTicker );
	mFrameController.setStartFrame( mStartFrame );
	mFrameController.update();
	
	mCameraPosition++;
}

void VideoPanApp::loopOffset() {
	mOffsetTicker += mOffsetVelocity;
	if(mOffsetTicker >= 480 || mOffsetTicker <= -480) {
		mOffsetVelocity *= -1;
	}
}

void VideoPanApp::draw()
{
	// clear out the window with black
	
	gl::clear( Color( 0, 0, 0 ) ); 
	gl::setMatricesWindow(getWindowSize(), true);
	
	
	
	mCam.setOrtho(0, 1200, 850, 0, -100, 100);
	gl::setMatrices( mCam );
	gl::rotate(mFrameRotation);
	mFrameController.draw();
	params::InterfaceGl::draw();
}


CINDER_APP_BASIC( VideoPanApp, RendererGl )
