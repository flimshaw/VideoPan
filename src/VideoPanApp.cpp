#include "cinder/app/AppBasic.h"
#include "cinder/ImageIO.h"
#include "cinder/gl/Texture.h"
#include "cinder/qtime/QuickTime.h"
#include "ParticleController.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class VideoPanApp : public AppBasic {
  public:
	void setup();
	void prepareSettings( Settings *settings );
	void mouseDown( MouseEvent event );
	void loadMovieFile( const std::string &path );
	void update();
	void draw();
	
	gl::Texture myImage;
	gl::Texture mMovieFrame;
	qtime::MovieGl mMovie;
	
	ParticleController mParticleController;
	
};

void VideoPanApp::prepareSettings( Settings *settings ){
    settings->setWindowSize( 800, 600 );
    settings->setFrameRate( 60.0f );
}

void VideoPanApp::setup()
{
	Url url( "http://libcinder.org/media/tutorial/paris.jpg" );
	myImage = gl::Texture( loadImage( loadUrl( url ) ) );
	
	string moviePath = getOpenFilePath();
	if( ! moviePath.empty() )
		loadMovieFile( moviePath );
	
	mParticleController = ParticleController( myImage );
	
	mParticleController.addParticles( 25 );
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
		mMovie.seekToStart();
		mMovie.play();
	}
	catch( ... ) {
		console() << "Unable to load the movie." << std::endl;
	}	
}

void VideoPanApp::mouseDown( MouseEvent event )
{
}

void VideoPanApp::update()
{
	if( mMovie ) {
		mMovieFrame = gl::Texture(mMovie.getTexture());
		gl::translate(1, 0);
	}
	mParticleController.update();
}

void VideoPanApp::draw()
{
	// clear out the window with black
	
	//gl::clear( Color( 0, 0, 0 ) ); 
	//gl::draw( myImage, getWindowBounds() );	
	if ( mMovieFrame ) {
		gl::draw( mMovieFrame, Rectf( 200, 200, 600, 500 ) );
	}
	
	glDisable( GL_TEXTURE_2D );
	glColor3f( 1, 1, 1 );
	mParticleController.draw();
}


CINDER_APP_BASIC( VideoPanApp, RendererGl )
