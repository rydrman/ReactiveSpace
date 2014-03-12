#pragma once
#include "IScene.h"
#include "Light.h"
#include "HexagonParticle.h"

class LightScene : public IScene
{
	ofImage m_hexImg;
	ofImage m_lightImg;
	ofImage m_fogImg;

	ofShader m_fogShader;
	ofVbo m_fogVbo;
	int m_fogInt;
	ofTexture m_lightsOnTexture;
	ofPixels m_lightsOnPixels;

	vector<Light> m_lights;

	float m_distanceToHands;
	
public:
	LightScene(vector<Particle*>* people, vector<Particle*>* hands);

	void Render();
	void Update(int deltaTime);

	void convertPeopleVector();
	Particle* addParticleOfProperType(ofVec3f _pos);

	~LightScene();
};