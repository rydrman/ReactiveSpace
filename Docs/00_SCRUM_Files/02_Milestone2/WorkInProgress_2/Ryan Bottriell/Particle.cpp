#include "Particle.h"

Particle::Particle(ofVec2f _pos)
{
	pos = _pos;
	vel = ofVec2f(0.f, 0.f);
	accel = ofVec2f(0.f, 0.f);
	maxSpeed = std::numeric_limits<float>::max();
	maxForce = std::numeric_limits<float>::max();
}

void Particle::applyForce(ofVec2f f)
{
	f.limit(maxForce);
	accel += f;
}
void Particle::applyForce(float x, float y)
{
	applyForce( ofVec2f(x, y) );
}

void Particle::update()
{
	vel += accel;
	vel.limit(maxSpeed);
	pos += vel;

	//reset accel
	accel *= 0;
}

void Particle::seek(ofVec2f target, float strength, bool slowToTarget, float* homeDistRatio)
{
	ofVec2f steerVec = ofVec2f(0.f, 0.f);
	ofVec2f desired = target - pos;
	float distSqrd = desired.lengthSquared();

	float homeRatio = 0.f;

	if (distSqrd > 0)
	{
		homeRatio = 1.f;
		desired.normalize();
		desired *= maxSpeed;

		if (slowToTarget && distSqrd < 10000)
		{
			homeRatio = distSqrd / 10000;
			desired *= homeRatio;
			if (desired.lengthSquared() < 1.f)
				desired.normalize();
		}

		steerVec = desired - vel;
		steerVec.limit(maxForce);
	}

	accel += steerVec * strength;
	*homeDistRatio = homeRatio;
}

Particle::~Particle()
{
}
