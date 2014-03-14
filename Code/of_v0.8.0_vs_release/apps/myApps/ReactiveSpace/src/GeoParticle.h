#pragma once
#include "particle.h"

struct GeoTriangle
{
	ofVec2f verts[3];
	ofVec2f texCoords[3];
};

class GeoParticle :
	public Particle
{
public:
	
	ofImage m_shapes;
	
	ofImage GeoShape;
	int GeoSize;
	int GeoTimer;
	int startTime;

	//for geometry
	float m_gradNum;
	int m_numVerts;
	ofVec2f* m_originalVerts;
	float* m_noiseOffsets;
	int m_numTriangles;
	GeoTriangle* m_trianlges;

	ofVec2f* m_currentVerts;
	ofVec2f* m_currentTexCoords;
	ofVbo m_vbo;

	GeoParticle(Particle _old);
	GeoParticle(ofVec3f _pos);

	void update(float timeScale);
	void draw();
	void explode();
	void setTriangles();
	void getRandomTexCoord(ofVec2f* coords);
	void countDown(int dTime);
	~GeoParticle(void);
};