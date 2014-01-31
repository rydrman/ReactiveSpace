#pragma once

#include "ofMain.h"

struct Particle {
	ofPoint pos;
	ofVec3f speed;
};

struct BirdParticle {
	ofVec2f pos;
	ofVec2f vel;
	ofVec2f accel;
	float rad;
	float color;
	float noiseX;
	float noiseY;
};