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
#include <list>

using namespace ci;
using namespace std;

class FrameController {
public:
	FrameController( const std::string &path );
	void update();
	void draw();
	void setup();
	void loadMovieFile( const std::string &path );
	void setFocalDistance( float focalDistance );
	
	
	std::list<FrameSlice>	mFrameSlices;
	qtime::MovieGl mVideo;
	
	int mFrameCount;
	int maxFrames;
	int mStartFrame;
	float mSliceWidth;
	float mPixelOffset;
	
	gl::Texture mTexture;
	
	float mFrameFocalDistance;
	float mFrameSpeed;
	float mFrameOffset;
};