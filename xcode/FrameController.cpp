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
		
		
		mVideo.setLoop( true, true );
		mVideo.play();
		mVideo.seekToFrame(mStartFrame);
		mVideo.setVolume(0);
		mVideo.stop();
		mFrameUpdateFlag = true;
	}
	catch( ... ) {
		console() << "Unable to load the movie." << std::endl;
	}
}

void FrameController::setFrameFocalDistance( float focalDistance )
{
	mFrameFocalDistance = focalDistance;
	mFrameUpdateFlag = true;
}

void FrameController::setStartFrame( int startFrame )
{
	mStartFrame = startFrame;
	mFrameUpdateFlag = true;
}

void FrameController::setFrameSpeed( float frameSpeed )
{
	mFrameSpeed = frameSpeed;
	mFrameUpdateFlag = true;
}

void FrameController::clearFrameSlices() 
{
	mFrameSlices.clear();
	buildFrameSlices();
}

void FrameController::buildFrameSlices()
{
	for(int i = 0; i < mMaxFrames; i++) {
		console() << getElapsedSeconds() << " queued frame " << i << std::endl;
		mFrames.push_back(mStartFrame + i);
	}
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

void FrameController::processVideoFrames()
{
	if ( ! mFrames.empty() ) {
        int frameNumber = mFrames.back();
        mFrames.pop_back();
		console() << getElapsedSeconds() << " thread created for frame " << frameNumber << std::endl;
        thread frameLoader(&FrameController::threadedLoad, this, frameNumber);
    }
    
    //completedLoadsMutex.lock();
    if ( !completedLoads.empty() ) {
		map<int, Surface>::iterator framejob;
		framejob = completedLoads.begin(); // get the first element
		completedLoads.erase(framejob); // and delete it from our loads map
		console() << getElapsedSeconds() << " creating texture " << std::endl;
		// we've got a frame loaded, it's time to create a frameSlice and append it to our list
		mFrameSlices.push_back( FrameSlice( gl::Texture(framejob->second), framejob->first, mFrameIndex, mFrameOffset, mFrameSpeed, mFrameFocalDistance ) );    
		console() << getElapsedSeconds() << framejob->second << " frameSlice created and appended" << std::endl;
		mFrameIndex++;
		//mFrameUpdateFlag = true;
	}
    //completedLoadsMutex.unlock();
	
}

void FrameController::threadedLoad( const int &frameNumber ) {
	console() << getElapsedSeconds() << " thread started " << std::endl;
	mVideo.seekToFrame(frameNumber);
	while( 1 ) {
		if( mVideo.checkNewFrame() ) {
			Surface surface = mVideo.getSurface();
			console() << getElapsedSeconds() << " surface created " << std::endl;
			// don't let the things in this block happen at the same time ever.
			//completedLoadsMutex.lock();
			completedLoads[frameNumber] = surface;

			//completedLoadsMutex.unlock();
			break;
		}
	}
	console() << getElapsedSeconds() << " thread completed " << std::endl;
}

void FrameController::update()
{
	if(mFrames.empty() && mFrameUpdateFlag) {
		console() << getElapsedSeconds() << " building frames " << std::endl;
		buildFrameSlices();
		mFrameUpdateFlag = false;
	}
	
	processVideoFrames();
	for( vector<FrameSlice>::iterator p = mFrameSlices.begin(); p != mFrameSlices.end(); ++p ){
		p->update();
	}
}

void FrameController::draw()
{
	for( vector<FrameSlice>::iterator p = mFrameSlices.begin(); p != mFrameSlices.end(); ++p ){
		p->draw();
	}
}
