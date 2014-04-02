#pragma once
#include "IScene.h"
#include "Light.h"
#include "HexagonParticle.h"

class LightScene : public IScene
{
	//for hex
	ofImage m_hexImgBorder;
	ofImage m_hexImgInner;
	ofImage m_hexLineImg;

	//for lights
	ofImage m_lightImg;
	ofImage m_lightTube;
	vector<Light> m_lights;

	//for background
	ofImage m_backgroundImg;
	ofSoundPlayer* pBackgroundSound;

	//for fog
	ofShader m_fogShader;
	ofVbo m_fogVbo;
	ofImage m_fogImg;
	ofFbo m_fogAlphaMask;
	ofImage m_lightAlpha;

	//for hands
	ofImage m_handsImage;
	vector<Particle*> m_closestHand; 
	Boolean m_connectedToHands;
	vector<HexagonParticle*> m_connectedParticles;

	
public:
	LightScene(vector<Particle*>* people, vector<Particle*>* hands, AudioManager* audioManager);

	void Render();
	void Update(float timeScale);

	void onLoad();
	void onUnload();

	void convertPeopleVector();
	Particle* addParticleOfProperType(ofVec3f _pos);

	~LightScene();
};