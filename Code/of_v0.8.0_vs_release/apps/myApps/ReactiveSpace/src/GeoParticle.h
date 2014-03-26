#pragma once
#include "particle.h"

struct GeoTriangle
{
	ofVec2f verts[3];
	ofVec2f texCoords[3];
	ofVec2f center;
	ofVec2f offset;
	float angle;
};

class GeoParticle :
	public Particle
{
public:
	ofSoundPlayer geoExplodeSound;
	ofImage m_shapes;
	
	int alphaExplode;
	ofImage GeoShape;
	int GeoSize;
	int GeoTimer;
	int startTime;
	int alphaTime;

	//for geometry
	int m_gradNum;
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
	bool m_isDead;
	float m_explodeTime;

	GeoParticle(ofVec3f _pos);

	void update(float timeScale);
	void draw(ofImage* mGrad, ofImage* explosionSprite);
	void explode();
	void countDown(float dTime, ofSoundPlayer geoExplosionSound, ofSoundPlayer geoExplosionSound2);
	ofColor getColor(int alpha = 255);

private:
	void setTriangles();
	void getRandomTexCoord(ofVec2f* coords);
	ofVec2f rotatePoint(ofVec2f point, ofVec2f center, float angle);
	
	~GeoParticle(void);
};

