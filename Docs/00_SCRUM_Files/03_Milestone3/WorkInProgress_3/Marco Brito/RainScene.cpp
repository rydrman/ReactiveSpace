#include "RainScene.h"
#define DEBUG_DRAW 1

static const float PARTICLE_DIST = 50;
static const float PARTICLE_DIST_SQRD = 2500;

RainScene::RainScene(vector<Particle*>* people, vector<Particle*>* hands)
: IScene(people, hands)
{

	int screenW = ofGetWidth();
	int screenH = ofGetHeight();

	for (int i = 0; i < s_numRainParticles; ++i)
	{
		m_rainParticles[i] = new Particle(ofVec2f(ofRandom(0, screenW), ofRandom(0, screenH)));
		m_rainParticles[i]->vel = ofVec2f(0.f, ofRandom(screenH * 0.005f, screenH * 0.01f));
	}

	float scale = screenH * screenW;
	float particleSize = sqrt(scale) / 50.f;

	particleUpdateOffset = false;

	m_gridWidth = floor(screenW / particleSize);
	m_gridHeight = floor(screenH / particleSize);
	m_gridSpacing = (float)screenW / (float)m_gridWidth;
	m_numParticles = m_gridHeight * m_gridWidth;

	m_particleList = new Particle [m_numParticles];

	int count = 0;
	for (int i = 0; i < m_gridWidth; ++i)
	{
		for (int j = 0; j < m_gridHeight; ++j)
		{
			Particle p = Particle(
				ofVec2f((i + 0.5) * m_gridSpacing, (j + 0.5) * m_gridSpacing));

			//set positions
			p.originalPos.x = (i + 0.5) * m_gridSpacing;
			p.originalPos.y = (j + 0.5) * m_gridSpacing;

			m_particleList[count] = p;
			++count;
		}
	}

	m_rainImage = ofImage("RainScene/Rain2.png");
	m_rainBackground = ofImage("RainScene/RainBackground.png");
}

void RainScene::Render()
{
	m_rainBackground.draw(0.f, 0.f, ofGetWidth(), ofGetHeight());

	ofFill();
	//ofSetColor(6, 297, 231);
	ofSetColor( 255 );
	for (int i = 0; i < s_numRainParticles; ++i)
	{
		ofPushMatrix();
			ofTranslate(m_rainParticles[i]->pos);
			//ofRotate(ofRadToDeg(atan2f(m_rainParticles[i]->vel.y, m_rainParticles[i]->vel.x)));
			//ofEllipse(0.f, 0.f, 0.f, 10.f, 10.f);
			m_rainImage.draw(0.f, 0.f, 0.f, 15.f, 30.f);
		ofPopMatrix();
	}


	//draw hands
	ofSetColor(255, 255, 255, 255);
	ofFill();
	for (vector<Particle*>::iterator h = pHandPositions->begin(); h != pHandPositions->end(); ++h)
	{
		ofPushMatrix();
		ofTranslate((*h)->pos);
		ofCircle(0.f, 0.f, 0.f, 20.f);

#ifdef DEBUG_DRAW
		ofSetColor(0, 0, 0, 255);
		ofFill();
		ofDrawBitmapString(ofToString((*h)->ID, 0), 0.f, 0.f);
		ofSetColor(0, 0, 0, 255);
#endif
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

	//check distance to hands
		bool edit = false;
		float dist;
		for (vector<Particle*>::iterator h = pHandPositions->begin(); h != pHandPositions->end(); ++h)
		{
			dist = ofDistSquared(p->pos.x, p->pos.y, (*h)->pos.x, (*h)->pos.y);
			if (dist < PARTICLE_DIST_SQRD)
			{
				//push away
				float angle = atan2f(p->pos.y - (*h)->pos.y, p->pos.x - (*h)->pos.x);
				float forceMag = 1.f * (1.f - dist / PARTICLE_DIST_SQRD);
				p->applyForce(cosf(angle) * forceMag, sinf(angle) * forceMag);
			}
		}
}

RainScene::~RainScene()
{
}
