#include "cinder/app/AppBasic.h"
#include "cinder/ImageIO.h"
#include "cinder/gl/Texture.h"
#include "cinder/qtime/QuickTime.h"
#include "cinder/Utilities.h"
#include "FrameController.h"
#include "ParticleEmitter.h"
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
	void mouseDrag( MouseEvent event );
	void mouseDown( MouseEvent event );
	void mouseWheel( MouseEvent event );

	gl::Texture mMovieFrame;
	qtime::MovieGl mMovie;
	string mMoviePath;
	int	mFrameRotation;
	
	CameraOrtho mCam;
	
	int mStartFrame;
	
	params::InterfaceGl mParams;
	
	FrameController mFrameController;

	ParticleEmitter particle_emitter;
	
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

	int mMouseStartX;
	int mCameraStartX;
};

void VideoPanApp::prepareSettings( Settings *settings ){
    settings->setWindowSize( 1200, 850 );
    settings->setFrameRate( 60.0f );
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
	mMaxFrames = 400;
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
	mParams.addParam( "Camera Position", &mCameraPosition, "step=1");
	mFrameController = FrameController( mMoviePath, mStartFrame, mFrameOffset, mFrameSpeed, mFrameFocalDistance, mMaxFrames );
	particle_emitter = ParticleEmitter( 50 );
}

void VideoPanApp::update()
{
	loopOffset();
	mFrameController.setFrameFocalDistance( mFrameFocalDistance );
	mFrameController.setFrameSpeed( mFrameSpeed );
	mFrameController.setFrameOffset( mOffsetTicker );
	mFrameController.setStartFrame( mStartFrame );
	mFrameController.update();
}

void VideoPanApp::mouseDrag( MouseEvent event ) {
	int mouseDist = mMouseStartX - event.getPos()[0];
    mCameraPosition = mCameraStartX + mouseDist;
}

void VideoPanApp::mouseDown( MouseEvent event ) {
    mMouseStartX = event.getPos()[0];
	mCameraStartX = mCameraPosition;
}

void VideoPanApp::mouseWheel( MouseEvent event) {
	float mouseInc = event.getWheelIncrement();
	mFrameFocalDistance += int(mouseInc);
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
	
	
	
	mCam.setOrtho(0 + mCameraPosition, 1200 + mCameraPosition, 850, 0, -100, 100);
	gl::setMatrices( mCam );
	gl::rotate(mFrameRotation);
	mFrameController.draw();
	params::InterfaceGl::draw();
}


CINDER_APP_BASIC( VideoPanApp, RendererGl )
