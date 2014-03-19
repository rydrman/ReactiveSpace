#pragma once
#include "IScene.h"
#include "Light.h"
#include "HexagonParticle.h"

class LightScene : public IScene
{
	ofImage m_hexImgBorder;
	ofImage m_hexImgInner;
	ofImage m_lightImg;
	ofImage m_lightTube;
	ofImage m_backgroundImg;
	

	//for fog
	ofShader m_fogShader;
	ofVbo m_fogVbo;
	ofImage m_fogImg;
	ofFbo m_fogAlphaMask;
	ofImage m_lightAlpha;

	ofImage m_handsImage;

	vector<Light> m_lights;

	vector<Particle*> closestHand; 
	
public:
	LightScene(vector<Particle*>* people, vector<Particle*>* hands);

	void Render();
	void Update(int deltaTime);

	void convertPeopleVector();
	Particle* addParticleOfProperType(ofVec3f _pos);

	~LightScene();
};