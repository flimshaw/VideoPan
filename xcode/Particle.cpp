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
#include "cinder/Area.h"
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
	frameSize = Vec2f(1280, 960);
	mVel	= Rand::randFloat( 5.0f );
	mRadius	= 3.0f;
	mTexture = newTexture;
	mFrameNumber = frameNumber;
	sliceWidth = 10;
	topLeft = Vec2f(0, (frameSize.y / 2) - (sliceWidth / 2));
	bottomRight = Vec2f(frameSize.x, (frameSize.y / 2) + (sliceWidth / 2) );
}

void Particle::setup() {
	
}

void Particle::updateTexture( gl::Texture newTexture ) 
{
	mTexture = newTexture;
}

void Particle::update()
{
	
	mLoc.x = 0;
	mLoc.y = - ((mFrameNumber * sliceWidth) - sliceWidth);
}

void Particle::draw()
{
	if( mTexture ) {
		Area cropArea(topLeft, bottomRight);
		gl::draw( mTexture, cropArea, Rectf( mLoc.x, mLoc.y, frameSize.x, mLoc.y + sliceWidth ) );
	}
}

