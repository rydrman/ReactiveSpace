#pragma once

#include "Particle.h"
#include "ofMain.h"


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

	BirdParticle(ofVec2f _pos, float _maxSpeed, float _maxForce);

	//for bird particle only: 
	//goes back to original grid position
	void seek();

	~BirdParticle();
};


