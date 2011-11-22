/*
 *  FrameSlice.cpp
 *  VideoPan
 *
 *  Created by Charles Hoey on 11/20/11.
 *  Copyright 2011 Flimshaw Industries. All rights reserved.
 *
 */

#include "FrameSlice.h"
#include "cinder/Rand.h"
#include "cinder/Area.h"
#include "cinder/gl/gl.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/Texture.h"

using namespace ci;
using namespace ci::app;

FrameSlice::FrameSlice( gl::Texture newTexture, int frameNumber, float frameOffset, float frameSpeed, float frameFocalDistance )
{
	mFrameNumber = frameNumber;
	mFrameOffset = frameOffset;
	mFrameSpeed = frameSpeed;
	mFrameFocalDistance = frameFocalDistance;
	mFrameLensAngle = 110;
	mFrameTexture = newTexture;
}

void FrameSlice::setPosition( ci::Vec2f newPos)
{
	mDestinationPosition = newPos;
}

void FrameSlice::setFrameTexture( gl::Texture newTexture )
{
	mFrameTexture = newTexture;
}

void FrameSlice::setFrameOffset( float frameOffset )
{
	mFrameOffset = frameOffset;
}

void FrameSlice::setup() 
{
	
}

void FrameSlice::update()
{
	// TODO: add some nice physics so we fly toward this instead of snapping to it
	mCurrentPosition = mDestinationPosition;
}

void FrameSlice::draw()
{
	if( mFrameTexture ) {
		gl::draw( mFrameTexture );
	}
}
