#pragma once

#include "ofMain.h"

class Particle
{
public:
	//generic data members
	ofVec3f pos;
	ofVec3f prevPos;
	ofVec3f vel;
	ofVec3f accel;
	ofVec2f originalPos;
	float restitution;
	float maxSpeed;
	float maxForce;
	float isHome;
	
	
	//for kinect / tracking
	int ID;
	int jointIndex;
	int timer;

	Particle(ofVec3f _pos);
	Particle();

	//useful functions
	void applyForce(ofVec3f f);
	void applyForce(float x, float y);
	void seek(ofVec3f target, float strength = 1.f, bool slowToTarget = false, float* homeDistRatio = new float);

	//update velocity
	void update(float timeScale = 1.f);

	~Particle();
};