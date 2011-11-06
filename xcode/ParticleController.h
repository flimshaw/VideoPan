/*
 *  ParticleController.h
 *  VideoPan
 *
 *  Created by Charles Hoey on 11/6/11.
 *  Copyright 2011 Flimshaw Industries. All rights reserved.
 *
 */

#pragma once
#include "Particle.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include <list>

using namespace ci;

class ParticleController {
public:
	ParticleController();
	ParticleController( gl::Texture newTexture );
	void update();
	void draw();
	void addParticles( int amt );
	void addParticle( gl::Texture newTexture );
	void removeParticles( int amt );
	
	std::list<Particle>	mParticles;
	
	int frameCount;
	int maxFrames;
	int startFrame;
	
	gl::Texture mTexture;
};