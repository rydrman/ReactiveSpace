#pragma once
#include "IScene.h"
#include "ParticleMain.h"

class GeoScene : public IScene
{
	Particle* m_Geometry;
	//ofImage geo1;
	//ofImage geo2;
	//ofImage geo3;
	ofImage geoBack;
	ofImage geoHands;

	ofImage m_gradients;

	int screenW;
	int screenH;
public:
	GeoScene(vector<Particle*>* people, vector<Particle*>* hands);

	virtual void Render();
	virtual void Update(int deltaTime);
	void convertPeopleVector();
	Particle* addParticleOfProperType(ofVec3f _pos);
	~GeoScene();
};

