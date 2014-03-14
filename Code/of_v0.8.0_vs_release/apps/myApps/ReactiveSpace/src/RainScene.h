#pragma once
#include "IScene.h"
#include "Particle.h"

class RainScene : public IScene
{

	//store particles
	Particle* m_rainParticles;

	//vector cloud for rain
	int m_rainFieldWidth;
	int m_rainFieldHeight;
	Particle* m_rainVectorField;
	float m_rainVectorFieldNorm;

	ofImage m_rainImage;
	ofImage m_rainBackground;

public:
	RainScene(vector<Particle*>* people, vector<Particle*>* hands);

	virtual void Render();
	virtual void Update(int timeScale);

	~RainScene();
};

