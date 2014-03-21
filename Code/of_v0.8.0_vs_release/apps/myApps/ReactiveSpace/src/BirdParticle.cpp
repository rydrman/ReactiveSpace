#include "BirdParticle.h"

BirdParticle::BirdParticle()
:Particle(ofVec2f(0.f, 0.f))
{
}

BirdParticle::BirdParticle(ofVec2f _pos, float _maxSpeed, float _maxForce)
	:Particle(_pos)
{
	maxSpeed = _maxSpeed;
	maxForce = _maxForce;
	isHome = false;
	homeDistRatio = 1.f;
}

void BirdParticle::update(float timeScale)
{
	Particle::update(timeScale);
}

void BirdParticle::update(vector<BirdParticle*>* angryParticles, float timeScale)
{
	ofVec2f steerVecSep = ofVec2f(0.f, 0.f);
	ofVec2f sumCoh = ofVec2f(0.f, 0.f);
	ofVec2f sumAlign = ofVec2f(0.f, 0.f);

	int count = 0;
	BirdParticle* angry;
	for (vector<BirdParticle*>::iterator p = angryParticles->begin(); p != angryParticles->end(); ++p)
	{
		angry = (*p);
		//look for close particles
		float distSqrd = ofDistSquared(angry->pos.x, angry->pos.y, pos.x, pos.y);
		if (distSqrd < s_neighbourDistSqrd)
		{
			//do flocking calculations
			//seperation
			if (distSqrd < s_desiredSeperationSqrd && distSqrd > 0)
			{
				steerVecSep += ((pos - angry->pos).normalize() / sqrt(distSqrd));
			}

			//cohesion
			sumCoh += angry->pos;

			//alignment
			sumAlign += angry->vel;

			++count;
		}
	}

	if (count > 0)
	{
		//seperation
		steerVecSep /= (float)count;
		steerVecSep = steerVecSep.normalize() * maxSpeed;
		steerVecSep -= vel;
		steerVecSep.limit(maxForce * 0.2f);

		//cohesion
		sumCoh /= (float)count;
		Particle::seek(sumCoh, 0.1f);

		//alingnment
		sumAlign /= (float)count;
		sumAlign.normalize();

		//add them all
		accel += steerVecSep + sumAlign;
		accel.limit(maxSpeed * (0.1f + 0.9f * homeDistRatio));
	}

	Particle::update(timeScale);
}

bool BirdParticle::seek()
{
	//homeDistRatio = 1.f;

	Particle::seek(originalPos, 1.f, true, &homeDistRatio);

	//check to see if it's home
	if (!isHome 
		&& homeDistRatio < 0.001)
	{
		//register as home
		isHome = true;
		return true;
	}
	return false;
}

BirdParticle::~BirdParticle()
{

}