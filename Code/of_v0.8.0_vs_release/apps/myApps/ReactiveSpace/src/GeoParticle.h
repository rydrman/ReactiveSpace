#pragma once
#include "particle.h"
class GeoParticle :
	public Particle
{
public:
	
	ofImage m_shapes;
	
	ofImage GeoShape;
	int GeoSize;
	int GeoTimer;
	int startTime;

	GeoParticle(Particle _old);
	GeoParticle(ofVec3f _pos);

	void draw();
	void explode();
	void countDown(int dTime);
	~GeoParticle(void);
};

