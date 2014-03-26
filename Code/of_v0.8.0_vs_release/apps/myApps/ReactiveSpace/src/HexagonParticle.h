#pragma once
#include "ofMain.h"
#include "Particle.h"

//static separation 
static const float s_desiredSepSqrd = 45000;
static const float s_neighDistSqrd = 90000;

class HexagonParticle 
	: public Particle {
		
public : 
	ofColor hexColor;
	float hexSize;
	float hexAlpha;
	float hexRotation;

	bool isConnected;
	float hexMaxSize;
	float hexGrowthRate;
	ofVec2f hexToHands;

	HexagonParticle(ofVec3f _pos);
	~HexagonParticle();

	void update();
	void separation(vector<HexagonParticle*>* connectedParticles);

};