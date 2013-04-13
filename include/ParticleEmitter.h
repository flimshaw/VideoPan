/*
 *  ParticleEmitter.h
 *  VideoPan
 *
 *  Created by Charles Hoey on 04/02/13.
 *  Copyright 2013 Charlie Hoey. All rights reserved.
 *
 */
#pragma once

class ParticleEmitter {
public:
	// methods
	ParticleEmitter();
	ParticleEmitter( int maxParticles );
	void update();
	void draw();
	void setup();

	// variables
	int mMaxParticles;
};