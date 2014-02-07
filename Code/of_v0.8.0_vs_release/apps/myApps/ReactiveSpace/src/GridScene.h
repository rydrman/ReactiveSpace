#pragma once

#include "IScene.h"
#include "ParticleMain.h"
#include <vector>

using std::vector;

class GridScene : public IScene
{
	int m_gridHeight;
	int m_gridWidth;
	int m_numParticles;
	float m_gridSpacing;
	bool particleUpdateOffset;

	//store particles
	BirdParticle* m_particleList;
	vector<BirdParticle*> m_angryParticles;

	//resources
	ofImage m_particleImage;
	ofVbo m_particleVbo;
	ofShader m_particleShader;

public:
	GridScene(vector<Particle>* people, vector<Vector4>* hands);
	
	virtual void Render();
	virtual void Update(int deltaTime);

	~GridScene();
};

