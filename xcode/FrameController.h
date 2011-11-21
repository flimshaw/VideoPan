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
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/qtime/QuickTime.h"
#include <list>

using namespace ci;
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
	
	// internal methods for calculating real world distances
	float getDistancePerFrame();
	float getTrueWidth();
	float getDistancePerPixel();
	float getFrameSliceWidth();
	void clearFrameSlices();
	void buildFrameSlices();
	void updateFrameSlices();
	
	
	std::list<FrameSlice>	mFrameSlices;
	qtime::MovieGl mVideo;
	
	int			mFrameCount;
	int			mMaxFrames;
	int			mStartFrame;
	float		mSliceWidth;
	float		mPixelOffset;
	int			mFramesPerSecond;
	boolean_t	mFrameUpdateFlag;
	
	gl::Texture mTexture;
	
	float mFrameFocalDistance;
	float mFrameSpeed;
	float mFrameOffset;
};