#include "RainScene.h"
#define DEBUG_DRAW 1

static const int s_numRainParticles = 1000;
static const ofVec2f s_gravity = ofVec2f(0.0, 0.98);

static const int s_vectorFieldDensity = 75;
static const float s_vectorFieldDensityInv = 1.f / s_vectorFieldDensity;

RainScene::RainScene(vector<Particle*>* people, vector<Particle*>* hands)
: IScene(people, hands)
{

	int screenW = ofGetWidth();
	int screenH = ofGetHeight();

	//create rain particles
	m_rainParticles = new Particle[s_numRainParticles];
	for (int i = 0; i < s_numRainParticles; ++i)
	{
		m_rainParticles[i] = Particle(ofVec2f(ofRandom(0, screenW), ofRandom(0, screenH)));
		m_rainParticles[i].vel = ofVec2f(0.f, ofRandom(screenH * 0.005f, screenH * 0.01f));
		m_rainParticles[i].maxSpeed = m_rainParticles[i].vel.length();
	}

	float scale = screenH * screenW;
	float particleSize = sqrt(scale) / 50.f;

	//create vector field
	m_rainFieldWidth = ofGetWidth() / s_vectorFieldDensity;
	m_rainFieldHeight = ofGetHeight() / s_vectorFieldDensity;
	m_rainVectorField = new Particle[m_rainFieldWidth * m_rainFieldHeight];
	m_rainVectorFieldNorm = m_rainFieldWidth * 0.01f;

	int count = 0;

	for (int i = 0; i < m_rainFieldWidth; ++i)
	{
		for (int j = 0; j < m_rainFieldHeight; ++j)
		{
			int pos = (i*m_rainFieldHeight) + j;
			m_rainVectorField[pos] = Particle(ofVec3f(i * s_vectorFieldDensity, j * s_vectorFieldDensity));
			m_rainVectorField[pos].vel = ofVec3f(m_rainVectorFieldNorm, 0.f);
			m_rainVectorField[pos].maxSpeed = s_vectorFieldDensity;
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
			ofTranslate(m_rainParticles[i].pos);
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

#ifdef DEBUG_DRAW
	//draw vector field
	ofPushMatrix();
	ofSetColor(180, 0, 0, 255);
	ofTranslate(s_vectorFieldDensity *0.5f, s_vectorFieldDensity * 0.5f);

	Particle* vector;
	for (int i = 0; i < m_rainFieldWidth; ++i)
	{
		for (int j = 0; j < m_rainFieldHeight; ++j)
		{
			vector = &m_rainVectorField[(i*m_rainFieldHeight) + j];
			float len = vector->vel.length();

			ofPushMatrix();
			ofTranslate(vector->pos);
			ofFill();
			ofRect(-2.f, -2.f, 2.f, 2.f);
			ofNoFill();
			ofRotate(ofRadToDeg(atan2f(vector->vel.y, vector->vel.x)));
			ofLine(0.f, 0.f, len * 10.f, 0.f);
			ofPopMatrix();
		}
	}
	ofPopMatrix();
#endif
}

void RainScene::Update(int timeScale)
{
	int screenH = ofGetHeight();

	//update vector field values from hands
	for (vector<Particle*>::iterator h = pHandPositions->begin(); h != pHandPositions->end(); ++h)
	{
		float distSqrd = (*h)->vel.lengthSquared();
		if (distSqrd > 0)
		{
			//find closest vector field position
			int posX = (int)(*h)->pos.x / s_vectorFieldDensity;
			int posY = (int)(*h)->pos.y / s_vectorFieldDensity;

			m_rainVectorField[(posX * m_rainFieldHeight) + posY].vel += (*h)->vel * timeScale * 0.1f;
		}
	}

	//update rain based on vector field
	Particle* p;
	for (int i = 0; i < s_numRainParticles; ++i)
	{
		p = &m_rainParticles[i];

		//get vector field nearbys
		ofVec3f targetVel = ofVec3f(0.f, 0.f);

		//calculate vector field that's close
		int fieldX = p->pos.x / s_vectorFieldDensity;
		int fieldY = p->pos.y / s_vectorFieldDensity;

		if (fieldX < 1) fieldX = 1;
		else if (fieldX > m_rainFieldWidth - 2) fieldX = m_rainFieldWidth - 2;
		if (fieldY < 1) fieldY = 1;
		else if (fieldY > m_rainFieldHeight - 2) fieldY = m_rainFieldHeight - 2;

		for (int i = -1; i < 2; ++i)
		{
			for (int j = -1; j < 2; ++j)
			{
				int pos = ((fieldX + i) * m_rainFieldHeight) + (fieldY + j);
				targetVel += m_rainVectorField[pos].vel;
			}
		}
		targetVel *= 0.11f;

		p->accel +=  s_gravity * timeScale + (targetVel - p->vel) * timeScale ;
		p->update();

		if (p->pos.y > screenH)
		{
			p->pos = ofVec2f(ofRandom(0.f, ofGetWidth()), 0.f);
			p->vel = ofVec2f(0.f, ofRandom(screenH * 0.005f, screenH * 0.01f));
		}
	}

	//apprach zero in vector field
	for (int i = 0; i < m_rainFieldWidth; ++i)
	{
		for (int j = 0; j < m_rainFieldHeight; ++j)
		{
			m_rainVectorField[(i*m_rainFieldHeight) + j].vel *= 0.95f;
		}
	}
	
}

void RainScene::convertPeopleVector()
{
	vector<Particle*> newPeople = vector<Particle*>();

	for (vector<Particle*>::iterator pOld = pPeople->begin(); pOld != pPeople->end(); ++pOld)
	{
		RainCloudParticle* p = new RainCloudParticle(**pOld._Ptr);
		newPeople.push_back(p);
	}
	*pPeople = newPeople;
}
Particle* RainScene::addParticleOfProperType(ofVec3f _pos)
{
	RainCloudParticle* p = new RainCloudParticle(_pos);
	pPeople->push_back(p);
	return p;
}

RainScene::~RainScene()
{
}
