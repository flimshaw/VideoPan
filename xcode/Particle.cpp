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

Particle::Particle( Vec2f loc, gl::Texture newTexture )
{
	mLoc	= loc;
	mVel	= Rand::randFloat( 5.0f );
	mRadius	= 3.0f;
	mTexture = newTexture;
}

void Particle::update()
{
	mLoc += mDir * mVel;
}

void Particle::draw()
{
	//gl::drawSolidCircle( mLoc, mRadius );
	gl::draw( mTexture, Rectf( 0, 0, Rand::randFloat( 505.0f ), Rand::randFloat( 505.0f ) ) );
}

