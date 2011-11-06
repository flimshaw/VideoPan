/*
 *  Particle.h
 *  VideoPan
 *
 *  Created by Charles Hoey on 11/6/11.
 *  Copyright 2011 Flimshaw Industries. All rights reserved.
 *
 */

#pragma once
#include "cinder/Channel.h"
#include "cinder/Vector.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/qtime/QuickTime.h"

#include <vector>

using namespace ci;

class Particle {
public:
	Particle();
	Particle( gl::Texture newTexture, int frameNumber );
	void update();
	void draw();
	void setup();
	
	ci::Vec2f	mLoc;
	ci::Vec2f	mDir;
	float		mVel;
	
	// variables for calculating width and crop
	float		trueWidth;
	float		focalDistance;
	float		lensAngle;
	float		frameSkip;
	float		mFrameNumber;
	
	gl::Texture mTexture;
	
	float		mRadius;
};