#pragma once

#include <vector>
#include "ParticleMain.h"

using std::vector;

class IScene
{
public:
	vector<Particle*>* pPeople;
	vector<Particle*>* pHandPositions;

	IScene(vector<Particle*>* people, vector<Particle*>* hands);
	
	virtual void Render() = 0;
	virtual void Update(int deltaTime) = 0;

	virtual void convertPeopleVector();
	virtual Particle* addParticleOfProperType(ofVec3f _pos);

	virtual void convertHandVector();
	virtual Particle* addHandOfProperType(ofVec3f _pos);

	void OnEnterPassiveUser();
	void OnUserInteraction();


	~IScene();
};

