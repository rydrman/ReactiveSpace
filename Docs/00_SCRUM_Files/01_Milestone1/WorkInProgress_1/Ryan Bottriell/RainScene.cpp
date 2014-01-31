#include "RainScene.h"


RainScene::RainScene(vector<Particle>* people, vector<Vector4>* hands)
: IScene(people, hands)
{
}

void RainScene::Render()
{
	ofBackground(0, 255);
}

void RainScene::Update(int deltaTime)
{
}

RainScene::~RainScene()
{
}
