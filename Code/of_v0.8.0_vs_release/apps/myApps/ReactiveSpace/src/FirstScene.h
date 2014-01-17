#pragma once

#include "IScene.h"
#include "Particle.cpp"
#include <vector>

using std::vector;

class FirstScene : public IScene
{
public:
	FirstScene(vector<Particle>* people);
	
	virtual void Render();
	virtual void Update();

	~FirstScene();
};

