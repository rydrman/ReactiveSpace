#include "GeoScene.h"


GeoScene::GeoScene(vector<Particle*>* people, vector<Particle*>* hands)
: IScene(people, hands)
{
}

void GeoScene::Render()
{
	ofBackground(0, 255);
}

void GeoScene::Update(int deltaTime)
{
}

GeoScene::~GeoScene()
{
}
