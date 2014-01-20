#include "FirstScene.h"
#include "ofMain.h"


FirstScene::FirstScene(vector<Particle>* people)
: IScene(people)
{

}

void FirstScene::Render()
{
	ofBackground(0);

	for (vector<Particle>::iterator p = pPeople->begin(); p != pPeople->end(); ++p)
	{
		ofSetColor(255);
		ofFill();
		ofCircle(p->pos, 10.f);

		//create connections
		ofPath connections = ofPath();
		connections.setStrokeColor(ofColor(255));
		connections.setStrokeWidth(3.f);
		connections.setMode(ofPath::POLYLINES);
		for (vector<Particle>::iterator p2 = pPeople->begin(); p2 != pPeople->end(); ++p2)
		{
			if (p < p2
				&& p->pos.squareDistance(p2->pos) < 80000)
			{
				connections.moveTo(p->pos);
				connections.lineTo(p2->pos);
			}
		}

		//draw connections
		connections.draw();
	}
}

void FirstScene::Update()
{

}

FirstScene::~FirstScene()
{
}
