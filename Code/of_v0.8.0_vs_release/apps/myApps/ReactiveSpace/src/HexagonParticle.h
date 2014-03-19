#pragma once
#include "ofMain.h"
#include "Particle.h"

//static separation 
static const float s_desiredSepSqrd = 100;
static const float s_neighDistSqrd = 20000;

class HexagonParticle 
	: public Particle {
		
public : 
	ofColor hexColor;
	float hexSize;
	float hexAlpha;
	float hexRotation;

	float hexMaxSize;
	float hexGrowthRate;
	ofVec2f hexToHands;

	HexagonParticle(Particle _old);
	HexagonParticle(ofVec3f _pos);
	~HexagonParticle();

	void separation(vector<HexagonParticle*>* connectedParticles);

};