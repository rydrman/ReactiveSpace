#pragma once
#include "particle.h"

class RainCloudParticle :
	public Particle
{
public:
	ofImage clouds;
	int CloudSize;

	RainCloudParticle(Particle _old);
	RainCloudParticle(ofVec3f _pos);

	void draw();

	~RainCloudParticle(void);
};

