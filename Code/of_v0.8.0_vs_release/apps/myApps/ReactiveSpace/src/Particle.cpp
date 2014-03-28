#include "Particle.h"

Particle::Particle(ofVec3f _pos)
{
	pos = _pos;
	prevPos = _pos;
	vel = ofVec3f(0.f, 0.f);
	accel = ofVec3f(0.f, 0.f);
	maxSpeed = std::numeric_limits<float>::max();
	maxForce = std::numeric_limits<float>::max();
	restitution = 1.f;
	timer = 0;
}

Particle::Particle()
{
	pos = ofVec3f(0.f, 0.f);
	prevPos = ofVec3f(0.f, 0.f);
	vel = ofVec3f(0.f, 0.f);
	accel = ofVec3f(0.f, 0.f);
	maxSpeed = std::numeric_limits<float>::max();
	maxForce = std::numeric_limits<float>::max();
	restitution = 1.f;
	timer = 0;
}

void Particle::applyForce(ofVec3f f)
{
	f.limit(maxForce);
	accel += f;
}
void Particle::applyForce(float x, float y)
{
	applyForce( ofVec3f(x, y) );
}

void Particle::update(float timeScale)
{
	vel += accel;
	vel *= restitution;
	vel.limit(maxSpeed);
	prevPos = pos;
	pos += vel * timeScale;

	//reset accel
	accel *= 0;
}

void Particle::seek(ofVec3f target, float strength, bool slowToTarget, float* homeDistRatio)
{
	ofVec3f steerVec = ofVec3f(0.f, 0.f);
	ofVec3f desired = target - pos;
	float distSqrd = desired.lengthSquared();

	float homeRatio = 0.f;

	if (distSqrd > 0)
	{
		homeRatio = 1.f;
		desired.normalize();
		desired *= maxSpeed;

		if (slowToTarget && distSqrd < 10000)
		{
			homeRatio = distSqrd / 10000.f;
			vel *= homeRatio;
			//desired *= homeRatio;
			//if (desired.lengthSquared() < 1.f)
				//desired.normalize();
		}

		steerVec = desired * strength * homeRatio;// -vel;
		steerVec.limit(maxForce);
	}
	//cout << vel << endl;
	//cout << steerVec << endl << endl;

	accel += steerVec;
	*homeDistRatio = homeRatio;
}

Particle::~Particle()
{
}
