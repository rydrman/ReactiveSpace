#pragma once
#include "IScene.h"

class LightScene : public IScene
{
public:
	LightScene(vector<Particle>* people, vector<Vector4>* hands);

	virtual void Render();
	virtual void Update(int deltaTime);

	~LightScene();
};

