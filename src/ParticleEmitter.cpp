// This is a class that emits particles.
// What should it look like?
#include "ParticleEmitter.h";
#include "stdio.h";
#include "cinder/app/AppBasic.h"

using namespace std;
using namespace ci;
using namespace ci::app;

ParticleEmitter::ParticleEmitter() {

}

ParticleEmitter::ParticleEmitter(int max_particles) {
	imax_particles = max_particles;
	setup();
}

void ParticleEmitter::setup() {
	
}