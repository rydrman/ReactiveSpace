#include "RainScene.h"


RainScene::RainScene(vector<Particle>* people, vector<Vector4>* hands)
: IScene(people, hands)
{

	int screenW = ofGetWidth();
	int screenH = ofGetHeight();

	for (int i = 0; i < s_numRainParticles; ++i)
	{
		m_rainParticles[i] = new Particle(ofVec2f(ofRandom(0, screenW), ofRandom(0, screenH)));
		m_rainParticles[i]->vel = ofVec2f(0.f, ofRandom(screenH * 0.005f, screenH * 0.01f));
	}
}

void RainScene::Render()
{
	ofBackground(0, 255);

	ofFill();
	ofSetColor(6, 297, 231);
	for (int i = 0; i < s_numRainParticles; ++i)
	{
		ofPushMatrix();
			ofTranslate(m_rainParticles[i]->pos);
			ofRotate(ofRadToDeg(atan2f(m_rainParticles[i]->vel.y, m_rainParticles[i]->vel.x)));
			ofEllipse(0.f, 0.f, 0.f, 10.f, 10.f);
		ofPopMatrix();
	}
}

void RainScene::Update(int deltaTime)
{
	int screenH = ofGetHeight();

	Particle* p;
	for (int i = 0; i < s_numRainParticles; ++i)
	{
		p = m_rainParticles[i];

		p->update();

		if (p->pos.y > screenH)
		{
			p->pos = ofVec2f(ofRandom(0.f, ofGetWidth()), 0.f);
			p->vel = ofVec2f(0.f, ofRandom(screenH * 0.005f, screenH * 0.01f));
		}
	}
}

RainScene::~RainScene()
{
}
