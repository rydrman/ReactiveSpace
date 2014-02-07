#include "Particle.h"

Particle::Particle(ofVec2f _pos)
{
	pos = _pos;
	vel = ofVec2f(0.f, 0.f);
	accel = ofVec2f(0.f, 0.f);
	maxSpeed = std::numeric_limits<float>::max();
	maxForce = std::numeric_limits<float>::max();
}

void Particle::applyForce(float x, float y)
{
	accel.x += x;
	accel.y += y;
}

void Particle::update()
{
	vel += accel;
	pos += vel;

	//reset accel
	accel *= 0;
}

void Particle::seek(ofVec2f target, bool slowToTarget)
{
	ofVec2f steerVec = ofVec2f(0.f, 0.f);
	ofVec2f desired = target - pos;
	float distSqrd = desired.lengthSquared();

	if (distSqrd > 0)
	{
		desired.normalize();
		desired *= maxSpeed;

		if (slowToTarget && distSqrd < 10000)
		{
			desired *= (distSqrd / 10000);
		}

		steerVec = desired - vel;
		steerVec.limit(maxForce);
	}

	accel += steerVec;
}

Particle::~Particle()
{
}
