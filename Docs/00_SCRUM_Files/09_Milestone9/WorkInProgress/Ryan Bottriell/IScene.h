#pragma once

#include <vector>
#include "ParticleMain.h"
#include "AudioManager.h"
#include "imageManager.h"

using std::vector;

class IScene
{
public:
	vector<Particle*>* pPeople;
	vector<Particle*>* pHandPositions;
	AudioManager* pAudioManager;
	imageManager* pImageManager;

	IScene(vector<Particle*>* people, vector<Particle*>* hands, AudioManager* audioManager, imageManager* imageManager);
	
	virtual void Render() = 0;
	virtual void Update(float timeScale) = 0;

	virtual void convertPeopleVector();
	virtual Particle* addParticleOfProperType(ofVec3f _pos);

	virtual void convertHandVector();
	virtual Particle* addHandOfProperType(ofVec3f _pos);

	virtual void onUnload();
	virtual void onLoad();

	void OnEnterPassiveUser();
	void OnUserInteraction();


	~IScene();
};

