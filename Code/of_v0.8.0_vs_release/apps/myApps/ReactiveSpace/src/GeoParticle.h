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
	int m_noiseOffset[9];
	ofVec2f m_verts[9];
	ofVec2f m_vboVerts[16];
	ofVec2f m_texCoords[16];
	ofVbo m_vbo;

	GeoParticle(Particle _old);
	GeoParticle(ofVec3f _pos);

	void update();
	void draw();
	void explode();
	void countDown(int dTime);
	~GeoParticle(void);
};

