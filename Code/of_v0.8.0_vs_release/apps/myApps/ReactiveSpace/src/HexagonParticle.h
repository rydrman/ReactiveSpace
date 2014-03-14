#pragma once
#include "ofMain.h"
#include "Particle.h"

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
};