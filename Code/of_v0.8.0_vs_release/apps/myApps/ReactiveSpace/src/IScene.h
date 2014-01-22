#pragma once

#include <vector>
#include "Particle.cpp"

using std::vector;

class IScene
{
public:
	vector<Particle>* pPeople;

	IScene(vector<Particle>* people);
	
	virtual void Render() = 0;
	virtual void Update(int deltaTime) = 0;

	void OnEnterPassiveUser();
	void OnUserInteraction();


	~IScene();
};

