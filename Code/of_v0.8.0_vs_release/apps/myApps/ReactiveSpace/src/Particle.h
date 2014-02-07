#pragma once

#include "ofMain.h"

class Particle
{
public:
	ofVec2f pos;
	ofVec2f vel;
	ofVec2f accel;
	float maxSpeed;
	float maxForce;

	Particle(ofVec2f _pos);

	//useful functions
	void applyForce(ofVec2f f);
	void applyForce(float x, float y);
	void seek(ofVec2f target, float strength = 1.f, bool slowToTarget = false, float* homeDistRatio = new float);

	//update velocity
	void update();

	~Particle();
};