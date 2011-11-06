/*
 *  Particle.cpp
 *  VideoPan
 *
 *  Created by Charles Hoey on 11/6/11.
 *  Copyright 2011 Flimshaw Industries. All rights reserved.
 *
 */

#include "Particle.h"
#include "cinder/Rand.h"
#include "cinder/gl/gl.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/Texture.h"

using namespace ci;

Particle::Particle()
{
}

Particle::Particle( gl::Texture newTexture, int frameNumber )
{
	mLoc	= Vec2f(0, 0);
	mVel	= Rand::randFloat( 5.0f );
	mRadius	= 3.0f;
	mTexture = newTexture;
	mFrameNumber = frameNumber;
}

void Particle::setup() {
	
}

void Particle::update()
{
	mLoc.x = (mFrameNumber * 100) - 400;
	mLoc.y = 200;
}

void Particle::draw()
{
	//gl::drawSolidCircle( mLoc, mRadius );
	if( mTexture ) {
		gl::draw( mTexture, Rectf( mLoc.x, mLoc.y, mLoc.x + 100, mLoc.y + 200 ) );
	}
}

