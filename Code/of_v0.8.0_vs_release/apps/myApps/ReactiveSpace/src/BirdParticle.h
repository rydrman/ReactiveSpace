#pragma once

#include "Particle.h"
#include "ofMain.h"

//static vars for flocking
static const float s_desiredSeperationSqrd = 200;
static const float s_neighbourDistSqrd = 10000;

class BirdParticle :
	public Particle
{

public:
	ofVec2f originalPos;
	float rad;
	float color;
	float noiseX;
	float noiseY;
	float mood;  //0 -> 1, angry -> happy
	float isHome;

	
	BirdParticle();
	BirdParticle(ofVec2f _pos, float _maxSpeed, float _maxForce);

	void update(vector<BirdParticle*>* angryParticles);

	//for bird particle only: 
	//goes back to original grid position
	//returns true if it's at home
	bool seek();

	~BirdParticle();
};


