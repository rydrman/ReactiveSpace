#include "IScene.h"

IScene::IScene(vector<Particle>* people, vector<Vector4>* hands)
{
	pPeople = people;
	pHandPositions = hands;
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
