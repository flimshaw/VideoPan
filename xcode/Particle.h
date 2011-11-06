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
	Particle( ci::Vec2f, gl::Texture newTexture );
	void update();
	void draw();
	
	ci::Vec2f	mLoc;
	ci::Vec2f	mDir;
	float		mVel;
	
	// variables for calculating width and crop
	float		trueWidth;
	float		focalDistance;
	float		lensAngle;
	float		frameSkip;
	
	gl::Texture mTexture;
	
	float		mRadius;
};