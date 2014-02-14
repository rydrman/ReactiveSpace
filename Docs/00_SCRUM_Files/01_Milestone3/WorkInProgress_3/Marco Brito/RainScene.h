#pragma once
#include "IScene.h"
#include "Particle.h"

static const int s_numRainParticles = 1000;
static const ofVec2f s_gravity = ofVec2f(0.0, 9.8);

class RainScene : public IScene
{
	int m_gridHeight;
	int m_gridWidth;
	int m_numParticles;
	float m_gridSpacing;
	bool particleUpdateOffset;

	//store particles
	Particle* m_particleList;

	Particle* m_rainParticles[s_numRainParticles];
	ofImage m_rainImage;
	ofImage m_rainBackground;

public:
	RainScene(vector<Particle*>* people, vector<Particle*>* hands);

	virtual void Render();
	virtual void Update(int deltaTime);

	~RainScene();
};

