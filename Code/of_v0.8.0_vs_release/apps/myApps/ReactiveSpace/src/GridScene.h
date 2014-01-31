#pragma once

#include "IScene.h"
#include "Particle.cpp"
#include <vector>

using std::vector;

class GridScene : public IScene
{
	//static vars
	int m_gridHeight;
	int m_gridWidth;
	float m_gridSpacing;
	vector<BirdParticle> m_particleList;
	BirdParticle** m_particleGrid;

	//resources
	ofImage* particleImage;
	ofVbo m_particleVbo;
	ofShader m_particleShader;

public:
	GridScene(vector<Particle>* people, vector<Vector4>* hands);
	
	virtual void Render();
	virtual void Update(int deltaTime);

	~GridScene();
};

