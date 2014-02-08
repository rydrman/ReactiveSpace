#pragma once
#include "IScene.h"
#include "Particle.h"

static const int s_numRainParticles = 5000;
static const ofVec2f s_gravity = ofVec2f(0.0, 9.8);

class RainScene : public IScene
{
	Particle* m_rainParticles[s_numRainParticles];

public:
	RainScene(vector<Particle*>* people, vector<Particle*>* hands);

	virtual void Render();
	virtual void Update(int deltaTime);

	~RainScene();
};

