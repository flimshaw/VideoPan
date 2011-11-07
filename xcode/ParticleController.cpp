/*
 *  ParticleController.cpp
 *  VideoPan
 *
 *  Created by Charles Hoey on 11/6/11.
 *  Copyright 2011 Flimshaw Industries. All rights reserved.
 *
 */

#include "ParticleController.h"

#include "cinder/app/AppBasic.h"
#include "cinder/Rand.h"
#include "cinder/Vector.h"
#include "ParticleController.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include <deque>

using namespace ci;
using std::list;
using std::vector;

ParticleController::ParticleController() {
	
}

ParticleController::ParticleController( gl::Texture newTexture )
{
	mTexture = newTexture;
	frameCount = 0;
	sliceWidth = 10;
}

void ParticleController::update()
{
	for( list<Particle>::iterator p = mParticles.begin(); p != mParticles.end(); ++p ){
		p->update();
	}
}

void ParticleController::draw()
{
	for( list<Particle>::iterator p = mParticles.begin(); p != mParticles.end(); ++p ){
		p->draw();
	}
}

void ParticleController::setWidth( float widthDelta )
{
	sliceWidth += widthDelta;
	for( list<Particle>::iterator p = mParticles.begin(); p != mParticles.end(); ++p ){
		p->setWidth( sliceWidth );
	}
}

void ParticleController::addParticle( gl::Texture newTexture ) {
	frameCount++;
	mTextures.push_front( newTexture );
	mParticles.push_back( Particle( &mTextures[0], frameCount ) );

}


void ParticleController::addParticles( int amt )
{
	for( int i=0; i<amt; i++ )
	{
		float x = Rand::randFloat( app::getWindowWidth() );
		float y = Rand::randFloat( app::getWindowHeight() );
	}
}

void ParticleController::updateParticles( gl::Texture newTexture ) {
	mTextures.pop_back();
	mTextures.push_front(newTexture);
	int particleCount = 0;
	for( list<Particle>::iterator p = mParticles.begin(); p != mParticles.end(); ++p ){
		p->updateTexture( &mTextures[particleCount] );
		particleCount++;
	}
}

void ParticleController::removeParticles( int amt )
{
	for( int i=0; i<amt; i++ )
	{
		mParticles.pop_back();
	}
}

