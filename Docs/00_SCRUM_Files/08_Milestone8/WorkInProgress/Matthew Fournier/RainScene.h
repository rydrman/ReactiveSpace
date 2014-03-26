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

	bool m_createNextInCloud;
	int whichStart;
	int m_createNextInCloudNum;
	int ratio;

	ofImage m_rainImage;
	ofImage m_rainBackground;
	ofImage m_cloudImage;

public:
	RainScene(vector<Particle*>* people, vector<Particle*>* hands);

	virtual void Render();
	virtual void Update(int timeScale);

	void addNewRainDrop(Particle* p);

	void convertPeopleVector();
	Particle* addParticleOfProperType(ofVec3f _pos);
	//void convertHandVector();
	//Particle* addHandOfProperType(ofVec3f _pos);

	~RainScene();
};
