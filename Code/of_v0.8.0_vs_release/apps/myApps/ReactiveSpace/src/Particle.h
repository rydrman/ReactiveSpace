#pragma once

#include "ofMain.h"

class Particle
{
public:
	ofVec3f pos;
	ofVec3f vel;
	ofVec3f accel;
	float maxSpeed;
	float maxForce;

	Particle(ofVec3f _pos);

	//useful functions
	void applyForce(ofVec3f f);
	void applyForce(float x, float y);
	void seek(ofVec3f target, float strength = 1.f, bool slowToTarget = false, float* homeDistRatio = new float);

	//update velocity
	void update();

	~Particle();
};