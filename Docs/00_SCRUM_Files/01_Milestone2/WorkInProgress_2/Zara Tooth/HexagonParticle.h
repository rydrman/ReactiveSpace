#pragma once
#include "ofMain.h"
#include "Particle.h"

class HexagonParticle 
	: public Particle {
		
public : 
	ofColor hexColor;


	HexagonParticle(ofVec2f _pos);
	~HexagonParticle();
};