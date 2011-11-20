#include "cinder/app/AppBasic.h"
#include "cinder/ImageIO.h"
#include "cinder/gl/Texture.h"
#include "cinder/qtime/QuickTime.h"
#include "cinder/Utilities.h"
#include "ParticleController.h"
#include "cinder/Camera.h"
#include "cinder/params/Params.h"


using namespace ci;
using namespace ci::app;
using namespace std;

class VideoPanApp : public AppBasic {
  public:
	void setup();
	void prepareSettings( Settings *settings );
	void mouseWheel( MouseEvent event );
	void loadMovieFile( const std::string &path );
	void update();
	void draw();
	
	gl::Texture myImage;
	gl::Texture mMovieFrame;
	qtime::MovieGl mMovie;
	
	CameraOrtho mCam;
	
	int maxParticles;
	int particleCount;
	
	int startFrame;
	
	params::InterfaceGl mParams;
	
	ParticleController mParticleController;
	
	float mParticleWidth;
	float mPixelOffset;
	
};

void VideoPanApp::prepareSettings( Settings *settings ){
    settings->setWindowSize( 800, 960 );
    settings->setFrameRate( 30.0f );
	
}

void VideoPanApp::setup()
{
	Url url( "http://libcinder.org/media/tutorial/paris.jpg" );
	myImage = gl::Texture( loadImage( loadUrl( url ) ) );
	mCam.setOrtho(-1, 1, -1, 1, -1, 1);
	
	string moviePath = "/Volumes/SPEEDY/desert_right.mov";
	if( ! moviePath.empty() )
		loadMovieFile( moviePath );
	maxParticles = 200;
	particleCount = 0;
	startFrame = 10000;
	
	mParticleWidth = 20;
	mPixelOffset = 0;
	mParams = params::InterfaceGl( "Particle Width", Vec2i( 225, 200 ) );
	mParams.addParam( "Slice Width", &mParticleWidth, "min=1.0 max=100.0 step=.1 keyIncr=s keyDecr=w");
	mParams.addParam( "Slice Offset", &mPixelOffset, "min=-400.0 max=400.0 step=1 keyIncr=x keyDecr=z");
	
	mParticleController = ParticleController( myImage );
}

void VideoPanApp::loadMovieFile( const string &moviePath )
{
	try {
		mMovie = qtime::MovieGl( moviePath );
		
		console() << "Dimensions:" << mMovie.getWidth() << " x " << mMovie.getHeight() << std::endl;
		console() << "Duration:  " << mMovie.getDuration() << " seconds" << std::endl;
		console() << "Frames:    " << mMovie.getNumFrames() << std::endl;
		console() << "Framerate: " << mMovie.getFramerate() << std::endl;
		console() << "Alpha channel: " << mMovie.hasAlpha() << std::endl;		
		console() << "Has audio: " << mMovie.hasAudio() << " Has visuals: " << mMovie.hasVisuals() << std::endl;
		
		
		mMovie.setLoop( true, true );
		mMovie.seekToFrame(24000);
		mMovie.setVolume(0);
		mMovie.play();
		mMovie.stop();
	}
	catch( ... ) {
		console() << "Unable to load the movie." << std::endl;
	}	
}

void VideoPanApp::mouseWheel( MouseEvent event )
{
	if( !event.isShiftDown() )
	{
		//mParticleController.setWidth( event.getWheelIncrement() );
	} else {
		//mMovie.stepForward();
	}
}

void VideoPanApp::update()
{
	if( mMovie.checkNewFrame() ) {
		mMovieFrame = gl::Texture(mMovie.getTexture());
		if( particleCount < maxParticles ) {
			mParticleController.addParticle( gl::Texture(mMovie.getTexture()) );
			particleCount++;
			mMovie.stepForward();
		}
	} else {
		mParticleController.setWidth(mParticleWidth);
		mParticleController.setPixelOffset(mPixelOffset);
		mParticleController.update();
	}
}

void VideoPanApp::draw()
{
	// clear out the window with black
	
	gl::clear( Color( 0, 0, 0 ) ); 
	
	gl::setMatricesWindow(getWindowSize(), true);

	gl::rotate(90);
	mParticleController.draw();
	params::InterfaceGl::draw();
}


CINDER_APP_BASIC( VideoPanApp, RendererGl )
