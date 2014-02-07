#pragma once
#include "IScene.h"
#include "Light.h"

class LightScene : public IScene
{
	ofPolyline m_hexShape;
	ofImage m_hexImg;
	ofImage m_lightImg;
	vector<Light> m_lights;

public:
	LightScene(vector<Particle>* people, vector<Vector4>* hands);

	virtual void Render();
	virtual void Update(int deltaTime);

	~LightScene();
};