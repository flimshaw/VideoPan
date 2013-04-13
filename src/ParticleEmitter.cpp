// This is a class that emits particles.
// What should it look like?
#include "ParticleEmitter.h";
#include "cinder/app/AppBasic.h"

using namespace std;
using namespace ci;
using namespace ci::app;

ParticleEmitter::ParticleEmitter() {

}

ParticleEmitter::ParticleEmitter(int maxParticles) {
	mMaxParticles = maxParticles;
	setup();
}

void ParticleEmitter::setup() {
	
}