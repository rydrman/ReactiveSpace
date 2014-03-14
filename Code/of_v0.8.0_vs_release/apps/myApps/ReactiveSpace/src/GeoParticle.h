#pragma once
#include "particle.h"

struct GeoTriangle
{
	ofVec2f verts[3];
	ofVec2f texCoords[3];
	ofVec2f center;
	ofVec2f offset;
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
	GeoTriangle* m_triangles;

	ofVec2f* m_currentVerts;
	ofVec2f* m_currentTexCoords;
	ofVbo m_vbo;

	//for explosion
	bool m_isExploding;
	int m_explodeTime;

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