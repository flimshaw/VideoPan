#include "cinder/app/AppBasic.h"
#include "cinder/ImageIO.h"
#include "cinder/gl/Texture.h"
#include "cinder/qtime/QuickTime.h"
#include "ParticleController.h"
#include "cinder/Camera.h"

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
	
	ParticleController mParticleController;
	
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
	//string moviePath = getOpenFilePath();
	string moviePath = "/Volumes/SPEEDY/desert_right.mov";
	if( ! moviePath.empty() )
		loadMovieFile( moviePath );
	maxParticles = 50;
	particleCount = 0;
	startFrame = 10000;
	
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
		mMovie.seekToFrame(12000);
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
		mParticleController.setWidth( event.getWheelIncrement() );
	} else {
		mMovie.stepForward();
	}
}

void VideoPanApp::update()
{
	if( mMovie.checkNewFrame() ) {
		mMovieFrame = gl::Texture(mMovie.getTexture());
		if( particleCount < maxParticles ) {
			mParticleController.addParticle( gl::Texture(mMovie.getTexture()) );
			particleCount++;
		}
		mParticleController.update();
		mMovie.stepForward();
	}
}

void VideoPanApp::draw()
{
	// clear out the window with black
	
	gl::clear( Color( 0, 0, 0 ) ); 
	//gl::rotate(90);
	gl::setMatricesWindow(getWindowSize(), true);
	if ( mMovieFrame ) {
		//gl::draw( mMovieFrame, Rectf( 200, 200, 600, 500 ) );
	}
	
	//glDisable( GL_TEXTURE_2D );
	//glColor3f( 1, 1, 1 );
	gl::rotate(90);
	mParticleController.draw();
}


CINDER_APP_BASIC( VideoPanApp, RendererGl )
