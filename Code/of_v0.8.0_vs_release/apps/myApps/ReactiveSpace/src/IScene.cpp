#include "IScene.h"

IScene::IScene(vector<Particle*>* people, vector<Vector4>* hands)
{
	pPeople = people;
	pHandPositions = hands;
}

void IScene::convertPeopleVector()
{
	vector<Particle*> newPeople = vector<Particle*>();

	for (vector<Particle*>::iterator pOld = pPeople->begin(); pOld != pPeople->end(); ++pOld)
	{
		Particle* p = new Particle(**pOld._Ptr);
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

void IScene::OnEnterPassiveUser()
{

}

void IScene::OnUserInteraction()
{

}

IScene::~IScene()
{
}
