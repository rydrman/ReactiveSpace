#include "BirdParticle.h"

BirdParticle::BirdParticle(ofVec2f _pos, float _maxSpeed, float _maxForce)
	:Particle(_pos)
{
	maxSpeed = _maxSpeed;
	maxForce = _maxForce;
}

void BirdParticle::seek()
{
	Particle::seek(originalPos, true);
}

BirdParticle::~BirdParticle()
{

}