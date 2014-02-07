#pragma once
#include "ofMain.h"
#include "Particle.h"

class HexagonParticle 
	: public Particle {
		
public : 
	ofColor hexColor;

	HexagonParticle(Particle _old);
	HexagonParticle(ofVec3f _pos);
	~HexagonParticle();
};