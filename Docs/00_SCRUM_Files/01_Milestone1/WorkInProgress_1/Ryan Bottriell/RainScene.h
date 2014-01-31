#pragma once
#include "IScene.h"

class RainScene : public IScene
{
public:
	RainScene(vector<Particle>* people, vector<Vector4>* hands);

	virtual void Render();
	virtual void Update(int deltaTime);

	~RainScene();
};

