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

	ofVec2f m_verts[4];
	ofVec2f m_texCoords[4];
	ofVbo m_vbo;

	GeoParticle(Particle _old);
	GeoParticle(ofVec3f _pos);

	void draw();
	void explode();
	void countDown(int dTime);
	~GeoParticle(void);
};

