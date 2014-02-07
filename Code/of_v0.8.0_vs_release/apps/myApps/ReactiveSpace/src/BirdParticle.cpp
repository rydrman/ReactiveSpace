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
}

void BirdParticle::update(vector<BirdParticle*>* angryParticles)
{
	ofVec2f steerVecSep = ofVec2f(0.f, 0.f);
	ofVec2f sumCoh = ofVec2f(0.f, 0.f);
	ofVec2f sumAlign = ofVec2f(0.f, 0.f);

	int count = 0;
	for (vector<BirdParticle*>::iterator p = angryParticles->begin(); p != angryParticles->end(); ++p)
	{
		//look for close particles
		float distSqrd = ofDistSquared(p[0]->pos.x, p[0]->pos.y, pos.x, pos.y);
		if (distSqrd < s_neighbourDistSqrd)
		{
			//do flocking calculations
			//seperation
			if (distSqrd < s_desiredSeperationSqrd && distSqrd > 0)
			{
				steerVecSep += ( (pos - p[0]->pos).normalize() / sqrt(distSqrd) );
			}

			//cohesion
			sumCoh += p[0]->pos;

			//alignment
			sumAlign += p[0]->vel;

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
		accel.limit(maxSpeed);
	}

	Particle::update();
}

bool BirdParticle::seek()
{
	float homeDistRatio = 1.f;
	Particle::seek(originalPos, 1.0f, true, &homeDistRatio);

	//check to see if it's home
	if (!isHome 
		&& homeDistRatio < 0.0001)
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