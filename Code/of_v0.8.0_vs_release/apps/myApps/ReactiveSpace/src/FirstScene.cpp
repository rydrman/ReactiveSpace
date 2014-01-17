#include "FirstScene.h"
#include "ofMain.h"


FirstScene::FirstScene(vector<Particle>* people)
: IScene(people)
{

}

void FirstScene::Render()
{
	ofBackground(0);

	for (vector<Particle>::iterator it = pPeople->begin(); it != pPeople->end(); ++it)
	{
		ofSetColor(255);
		ofFill();
		ofCircle(it->pos, 10.f);
	}
}

void FirstScene::Update()
{

}

FirstScene::~FirstScene()
{
}
