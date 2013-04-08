/*
 *  FrameController.cpp
 *  VideoPan
 *
 *  Created by Charles Hoey on 11/20/11.
 *  Copyright 2011 Flimshaw Industries. All rights reserved.
 *
 */
#include "FrameController.h"
#include "cinder/app/AppBasic.h"
#include "cinder/Vector.h"
#include "FrameSlice.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/Utilities.h"
#include "cinder/qtime/QuickTime.h"

#include "cinder/ImageIo.h"
#include "cinder/Thread.h"
#include <vector>
#include <map>
#include <unordered_map>

#define PI 3.14159265
#define TAN_110 .70020754
#define SCREEN_WIDTH 960;

using namespace ci;
using namespace ci::app;
using namespace std;
using std::list;
using std::vector;

FrameController::FrameController()
{
	
}

FrameController::FrameController( const string &moviePath, int startFrame, float frameOffset, float frameSpeed, float frameFocalDistance, int maxFrames )
{

	// let's fast track this with some default values here
	mMaxFrames = maxFrames;
	mFramesPerSecond = 30;
	mStartFrame = startFrame;
	mFrameFocalDistance = frameFocalDistance;
	mFrameSpeed = frameSpeed;
	mPixelOffset = 0;
	mFrameIndex = 1;
	mLastFrameNumber = -100;
	mFrameLoading = false; // by default, we're not loading any frames, capish?
	mVideoReady = false;
	
	loadMovieFile( moviePath );
}

void FrameController::loadMovieFile( const string &moviePath )
{
	try {
		mVideo = qtime::MovieSurface( moviePath );
		
		console() << "Dimensions:" << mVideo.getWidth() << " x " << mVideo.getHeight() << std::endl;
		console() << "Duration:  " << mVideo.getDuration() << " seconds" << std::endl;
		console() << "Frames:    " << mVideo.getNumFrames() << std::endl;
		console() << "Framerate: " << mVideo.getFramerate() << std::endl;
		console() << "Alpha channel: " << mVideo.hasAlpha() << std::endl;		
		console() << "Has audio: " << mVideo.hasAudio() << " Has visuals: " << mVideo.hasVisuals() << std::endl;
		
		// get our movie loaded, then pause it and trigger our frame update flag to start harvesting frames
		mVideo.setLoop( true, true );
		mVideo.play();
		mVideo.setVolume(0);
		mVideo.stop();
		
		mVideoReady = true;
	}
	catch( ... ) {
		console() << "Unable to load the movie." << std::endl;
	}
}

void FrameController::setFrameFocalDistance( float focalDistance )
{
	mFrameFocalDistance = focalDistance;
	
	// FOR NOW we will calculate all this stuff in the here and now
	float frameSliceWidth = getFrameSliceWidth();
	setFrameSliceWidth( frameSliceWidth );
}

void FrameController::setStartFrame( int startFrame )
{
	if(startFrame != mStartFrame) {
		mStartFrame = startFrame;
		mFrameSlices.clear();
		mFrameQueue.clear();
		mFrameUpdateFlag = true;
		mFrameIndex = 0;
	}
}

void FrameController::setMaxFrames( int maxFrames ) {
	mMaxFrames = maxFrames;
}

void FrameController::setFrameOffset( float frameOffset )
{
	mFrameOffset = frameOffset;
	setFrameSliceOffset( mFrameOffset );
}

void FrameController::setFrameSliceOffset( int frameOffset ) {
	for( vector<FrameSlice>::iterator p = mFrameSlices.begin(); p != mFrameSlices.end(); ++p ){
		p->setFrameOffset( frameOffset );
	}	
}

void FrameController::setFrameSpeed( float frameSpeed )
{
	mFrameSpeed = frameSpeed;
}

void FrameController::clearFrameSlices() 
{
	mFrameSlices.clear();
	buildFrameSlices();
}

void FrameController::buildFrameSlices()
{
	int frameMultiplier = 1;
	for(int i = 0; i < mMaxFrames; i++) {
		mFrameQueue.push_back(mStartFrame + (i * frameMultiplier));
	}
	mVideo.seekToFrame(mStartFrame);
}
		
float FrameController::getDistancePerFrame()
{
	return (mFrameSpeed / (3600 * mFramesPerSecond)) * 5280;
}

float FrameController::getDistancePerPixel()
{
	return getTrueWidth() / SCREEN_WIDTH;
}
			
float FrameController::getTrueWidth()
{
	return ((mFrameFocalDistance / TAN_110) * 2);
}

float FrameController::getFrameSliceWidth()
{
	return getDistancePerPixel() / getDistancePerFrame();
}

void FrameController::setFrameSliceWidth( float newWidth ) {
	for( vector<FrameSlice>::iterator p = mFrameSlices.begin(); p != mFrameSlices.end(); ++p ){
		p->setFrameWidth( newWidth );
	}
}

void FrameController::setCameraPosition( int cameraPosition ) {
	mCameraPosition = cameraPosition;
}

void FrameController::processVideoFrames()
{    
	// if we have jobs that have been completed by our frameloader thread, deal with them
    if ( !completedLoads.empty() ) {
		// map this for now to get the key value
		map<int, Surface>::iterator framejob;
		
		// get the first element from our completed stack
		framejob = completedLoads.begin();

		// create a new slice with the video texture
		FrameSlice slice = FrameSlice( gl::Texture(framejob->second), framejob->first, mFrameOffset, mFrameSpeed, mFrameFocalDistance );
		slice.setFrameWidth(getFrameSliceWidth());
		// map the frame number to our slice for future reference
		mFrameMap[framejob->first] = &slice;

		// append this to our mFrameSlices list, may it last a thousand years
		mFrameSlices.push_back( slice );    

		// set our last frame number flag
		mLastFrameNumber = framejob->first;

		// erase this job from our complete loads list and make way for the next one
		completedLoads.erase(framejob);
	}
}

// separate threaded function that will be retrieving textures from our video file
void FrameController::threadedLoad( const int &frameNumber ) {	
	// if we're loading the frame directly after the previous frame, stepForward, it's faster
	if(mLastFrameNumber == frameNumber - 1) {
		mVideo.stepForward();
	} else {
		mVideo.seekToFrame(frameNumber); // jump to the frame number specified
	}
	
	while( 1 ) { // then loop like mad waiting for a video file to load
		if( mVideo.checkPlayable() == true && mVideo.checkNewFrame() == true ) { // if we've arrived at a new frame

			// get a surface from our video frame
			Surface frameSurface = mVideo.getSurface();

			// load this onto our object's array, we need to load the texture in our main thread
			completedLoads[frameNumber] = frameSurface; 

			// lastly set our loading flag to false
			mFrameLoading = false;

			// and leave the loop, freeing the next update() firing to fire another thread
			break;
		}
	}
}

// check to see if there is a slot waiting for a new video
int FrameController::getNextFrame() {

	// if we are loading a frame, return -1 immediately and skip some work
	if(mFrameLoading || mFrameSlices.size() >= mMaxFrames ) {
		return -1;
	}

	// get the first and last frames that should be visible given our camera position
	int firstFrame = abs(int(mCameraPosition / getFrameSliceWidth()));
	int lastFrame = firstFrame + 500;

	// find the first unloaded frame available and return it
	for( int i = firstFrame; i < lastFrame; i++ ) {
		if(mFrameMap.find(i) == mFrameMap.end()) {
			return i;
		}
	}

	// otherwise, return -1
	return -1;
	
}

void FrameController::loadFrame(int frameNumber) {
	// set our flag so we don't ever fire more than 1
	mFrameLoading = true;
	// and bombs away, let's fire off a thread to load this frame for us
	thread frameLoader(&FrameController::threadedLoad, this, frameNumber); // and spawn our loader thread
}

void FrameController::update()
{	

	// see if there are any new particles to ready to draw
	processVideoFrames();

	// if the video player is ready
	if(mVideoReady) {
		// if there is a frame available
		int nextFrame = getNextFrame();
		if(nextFrame > -1) {
			// load another frame
			loadFrame(nextFrame);
		}
	}

	vector<FrameSlice>::iterator p = mFrameSlices.begin();
	// loop through all the elements on our list
	for( ; p != mFrameSlices.end(); ){
		// clear out the dead
		if(p->isDead()) {
			mFrameMap.erase(p->getFrameNumber());
			p = mFrameSlices.erase(p);
		// and update the living
		} else {
			p->update(mCameraPosition);
			++p;
		}
	}
}

void FrameController::draw()
{
	// draw all of our frames
	for( vector<FrameSlice>::iterator p = mFrameSlices.begin(); p != mFrameSlices.end(); ++p ){
		p->draw();
	}
}
