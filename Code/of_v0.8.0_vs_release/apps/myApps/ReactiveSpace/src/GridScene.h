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
	float m_particleSize;
	bool particleUpdateOffset;

	//store particles
	BirdParticle* m_particleList;
	vector<BirdParticle*> m_angryParticles;

	//resources
	ofImage m_backgroundImage;
	ofImage m_particleImage;
	ofVbo m_particleVbo;
	ofShader m_particleShader;
	ofSoundPlayer* gridBackSound;

public:
	GridScene(vector<Particle*>* people, vector<Particle*>* hands, AudioManager* audioManager);
	
	virtual void Render();
	virtual void Update(float timeScalee);

	virtual void onUnload();

	//switching between scenes
	void onLoad();


	~GridScene();
};

