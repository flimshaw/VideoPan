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

FrameSlice::FrameSlice( gl::Texture newTexture, int frameNumber, int frameIndex, float frameOffset, float frameSpeed, float frameFocalDistance )
{
	mFrameNumber = frameNumber;
	mFrameIndex = frameIndex;
	mFrameOffset = frameOffset;
	mFrameSpeed = frameSpeed;
	mFrameFocalDistance = frameFocalDistance;
	mFrameLensAngle = 110;
	mWidth = 10;
	mFrameTexture = newTexture;
	mFrameSize = Vec2f(1280, 960); // size of the raw images
}

void FrameSlice::setPosition( ci::Vec2f newPos)
{
	mViewPosition = newPos;
}

void FrameSlice::setFrameTexture( gl::Texture newTexture )
{
	mFrameTexture = newTexture;
}

void FrameSlice::setFrameOffset( float frameOffset )
{
	mFrameOffset = frameOffset;
}

void FrameSlice::setFrameWidth( float newWidth ) {
	mWidth = newWidth;
}

// this will translate a standard x, y viewport position to an actual spatial position
void FrameSlice::translatePosition( Vec2f mPosition )
{
	// adjust for the rotation of the GL space
	mTruePosition = Vec2f(mPosition.y, -mPosition.x);
}

void FrameSlice::updateCropArea()
{
	// updates the rectangle used to crop out the display area of our slice.
	Vec2f topLeft = Vec2f(0, ((mFrameSize.y/2) - (mWidth/2)) + mFrameOffset);
	Vec2f bottomRight = Vec2f(mFrameSize.x, (mFrameSize.y / 2) + (mWidth / 2));
	mCropArea = Area(topLeft, bottomRight);
}

void FrameSlice::setup() 
{
	
}

void FrameSlice::update()
{
	// TODO: add some nice physics so we fly toward this instead of snapping to it
	translatePosition( Vec2f((mWidth * mFrameIndex), 0) );
	updateCropArea();
}

void FrameSlice::draw()
{
	Rectf newPos = Rectf( mTruePosition.x, mTruePosition.y, mTruePosition.x + mFrameSize.x, mTruePosition.y + mWidth );
	if( mFrameTexture ) {
		gl::draw( mFrameTexture, mCropArea, newPos );
	}
}
