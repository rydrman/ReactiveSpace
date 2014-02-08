#pragma once
#include "IScene.h"

class GeoScene : public IScene
{
public:
	GeoScene(vector<Particle*>* people, vector<Particle*>* hands);

	virtual void Render();
	virtual void Update(int deltaTime);

	~GeoScene();
};

