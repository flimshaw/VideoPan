/*
 *  FrameController.h
 *  VideoPan
 *
 *  Created by Charles Hoey on 11/20/11.
 *  Copyright 2011 Flimshaw Industries. All rights reserved.
 *
 */

#pragma once
#include "FrameSlice.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/qtime/QuickTime.h"
#include "cinder/ImageIo.h"
#include "cinder/Thread.h"
#include <list>
#include <vector>
#include <map>

using namespace ci;
using namespace ci::app;
using namespace std;

class FrameController {
public:
	FrameController();
	FrameController( const string &moviePath, int startFrame, float frameOffset, float frameSpeed, float frameFocalDistance, int maxFrames );
	void update();
	void draw();
	void setup();
	void loadMovieFile( const std::string &path );
	void setFrameFocalDistance( float focalDistance );
	void setFrameSpeed( float frameSpeed );
	void setStartFrame( int startFrame );
	void setCameraPosition( int cameraPosition );

	// internal methods for calculating real world distances
	float getDistancePerFrame();
	float getTrueWidth();
	float getDistancePerPixel();
	float getFrameSliceWidth();
	void clearFrameSlices();
	void buildFrameSlices();
	void updateFrameSlices();
	void setFrameSliceWidth( float newWidth );
	void setFrameSliceOffset( int frameOffset );
	void setFrameOffset( float frameOffset );

	
	// internal methods for working out shit
	void processVideoFrames();
	
	// this is our queue of frames we want to load:
    std::deque<int> mFrameQueue;

	// here's our flag that tells us if we are loading a frame in the background
	// if we are, we shouldn't spawn any more loader threads
	bool mFrameLoading;
    
	// this is our queue of things that are done loading
    map<int, Surface> completedLoads;
	
	// this is the function that will be loaded in a thread
    void threadedLoad(const int &frameNumber);
	
	
	vector<FrameSlice>	mFrameSlices;
	qtime::MovieSurface mVideo;
	
	int			mFrameCount;
	int			mFrameIndex;
	int			mCameraPosition;
	int			mMaxFrames;
	int			mStartFrame;
	float		mSliceWidth;
	float		mPixelOffset;
	int			mFramesPerSecond;
	bool	mFrameUpdateFlag;
	
	gl::Texture mTexture;
	
	float mFrameFocalDistance;
	float mFrameSpeed;
	int mFrameOffset;
};