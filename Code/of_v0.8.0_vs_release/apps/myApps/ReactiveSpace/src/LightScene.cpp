#include "LightScene.h"


LightScene::LightScene(vector<Particle>* people, vector<Vector4>* hands)
: IScene(people, hands)
{
}

void LightScene::Render()
{
	ofBackground(0, 255);
}

void LightScene::Update(int deltaTime)
{
}

LightScene::~LightScene()
{
}
