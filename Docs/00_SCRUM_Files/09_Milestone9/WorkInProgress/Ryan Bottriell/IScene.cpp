#include "IScene.h"

IScene::IScene(vector<Particle*>* people, vector<Particle*>* hands, AudioManager* audioManager)
{
	pPeople = people;
	pHandPositions = hands;
	pAudioManager = audioManager;
}

void IScene::onUnload()
{
}

void IScene::onLoad()
{
}

void IScene::convertPeopleVector()
{
	vector<Particle*> newPeople = vector<Particle*>();

	for (vector<Particle*>::iterator pOld = pPeople->begin(); pOld != pPeople->end(); ++pOld)
	{
		Particle* p = new Particle((*pOld)->pos);
		newPeople.push_back(p);
	}
	*pPeople = newPeople;
}

Particle* IScene::addParticleOfProperType(ofVec3f _pos)
{
	Particle* p = new Particle(_pos);
	pPeople->push_back(p);
	return p;
}

void IScene::convertHandVector()
{
	vector<Particle*> newHands = vector<Particle*>();

	for (vector<Particle*>::iterator hOld = pHandPositions->begin(); hOld != pHandPositions->end(); ++hOld)
	{
		Particle* h = new Particle((*hOld)->pos);
		newHands.push_back(h);
	}
	*pHandPositions = newHands;
}

Particle* IScene::addHandOfProperType(ofVec3f _pos)
{
	Particle* h = new Particle(_pos);
	pHandPositions->push_back(h);
	return h;
}

void IScene::OnEnterPassiveUser()
{

}

void IScene::OnUserInteraction()
{

}

IScene::~IScene()
{
}
