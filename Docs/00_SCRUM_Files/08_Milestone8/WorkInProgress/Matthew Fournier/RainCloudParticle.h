#pragma once
#include "particle.h"

class RainCloudParticle :
	public Particle
{
public:
	ofImage* m_cloudImage;
	ofVec2f m_size;

	RainCloudParticle(ofVec3f _pos);

	void draw();
	int rando;
	void addRainDrop(Particle* p);

	~RainCloudParticle(void);
};

