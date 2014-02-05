#pragma once

#include "ofMain.h"

struct Particle {
	ofVec2f pos;
	ofVec2f vel;
	ofVec2f accel;
};

struct BirdParticle :
	Particle{
	ofVec2f originalPos;
	float rad;
	float color;
	float noiseX;
	float noiseY;
	float mood;  //0 -> 1, angry -> happy
};

static void particle_applyForce(Particle* p, float x, float y)
{
	p->accel.x += x;
	p->accel.y += y;
}

static void particle_updatePosition(Particle* p)
{
	p->vel += p->accel;
	p->pos += p->vel;
}