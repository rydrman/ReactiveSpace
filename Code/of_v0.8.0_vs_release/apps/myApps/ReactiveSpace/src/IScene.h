#pragma once

#include <vector>
#include "ParticleMain.h"
#include <Windows.h>
#include <Ole2.h>
#include <NuiApi.h>

using std::vector;

class IScene
{
public:
	vector<Particle>* pPeople;
	vector<Vector4>* pHandPositions;

	IScene(vector<Particle>* people, vector<Vector4>* hands);
	
	virtual void Render() = 0;
	virtual void Update(int deltaTime) = 0;

	void OnEnterPassiveUser();
	void OnUserInteraction();


	~IScene();
};

