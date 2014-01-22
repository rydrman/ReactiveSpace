#pragma once

#include "ofMain.h"

struct Particle {
	ofPoint pos;
	ofVec3f speed;
};

struct BirdParticle {
	ofPoint pos;
	ofVec3f speed;
	float rad;
	float color;
	float noiseX;
};