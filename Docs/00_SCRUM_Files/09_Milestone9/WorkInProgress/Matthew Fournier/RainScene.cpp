#include "RainScene.h"
#define DEBUG_DRAW 1

static const int s_numRainParticles = 1000;
static const ofVec2f s_gravity = ofVec2f(0.0, 2.f);

static const int s_vectorFieldDensity = 75;
static const float s_vectorFieldDensityInv = 1.f / s_vectorFieldDensity;

RainScene::RainScene(vector<Particle*>* people, vector<Particle*>* hands)
: IScene(people, hands)
{

	int screenW = ofGetWidth();
	int screenH = ofGetHeight();
	whichStart = 0;
	m_createNextInCloud = false;
	m_createNextInCloudNum = 0;

	//create rain particles
	m_rainParticles = new Particle[s_numRainParticles];
	for (int i = 0; i < s_numRainParticles; ++i)
	{
		m_rainParticles[i] = Particle(ofVec2f(ofRandom(0, screenW), ofRandom(0, screenH)));
		m_rainParticles[i].vel = ofVec2f(0.f, ofRandom(screenH * 0.005f, screenH * 0.01f));
		m_rainParticles[i].maxSpeed = ofRandom(screenH * 0.003f, screenH * 0.009f);
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
			m_rainVectorField[pos].maxSpeed = s_vectorFieldDensity * 0.2f;
		}
	}

	m_rainImage.loadImage("RainScene/RainDrops.png");
	m_rainBackground.loadImage("RainScene/RainBackground.png");
	m_cloudImage.loadImage("RainScene/Clouds_spreadsheet.png");
}

void RainScene::Render()
{
	m_rainBackground.draw(0.f, 0.f, ofGetWidth(), ofGetHeight());

	//draw rain
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

	//draw clouds / people
	RainCloudParticle* rc;
	for (vector<Particle*>::iterator p = pPeople->begin(); p != pPeople->end(); ++p)
	{
		rc = (RainCloudParticle*)(*p);
		rc->draw();
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
			m_rainVectorField[(posX * m_rainFieldHeight) + posY].vel.limit( m_rainVectorField[(posX * m_rainFieldHeight) + posY].maxSpeed );
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

		if (fieldX < 2) fieldX = 2;
		else if (fieldX > m_rainFieldWidth - 3) fieldX = m_rainFieldWidth - 3;
		if (fieldY < 2) fieldY = 2;
		else if (fieldY > m_rainFieldHeight - 3) fieldY = m_rainFieldHeight - 3;

		for (int i = -2; i < 3; ++i)
		{
			for (int j = -2; j < 3; ++j)
			{
				int pos = ((fieldX + i) * m_rainFieldHeight) + (fieldY + j);
				targetVel += (3.f - std::max(abs(i), abs(j))) * m_rainVectorField[pos].vel;
			}
		}

		
		targetVel *= 0.029f ;

		p->applyForce(s_gravity.x * timeScale, s_gravity.y * timeScale);
		
		

		p->applyForce(targetVel.x * timeScale, targetVel.y * timeScale);
	

		p->update();

		if (p->pos.y > screenH)
		{
			addNewRainDrop(p);
		}
	}

	//apprach zero in vector field
	for (int i = 0; i < m_rainFieldWidth; ++i)
	{
		for (int j = 0; j < m_rainFieldHeight; ++j)
		{
			m_rainVectorField[(i*m_rainFieldHeight) + j].vel *= 0.995f + 0.004f * (1.f - timeScale);
		}
	}
	
}

void RainScene::addNewRainDrop(Particle* p)
{
	int numClouds = pPeople->end() - pPeople->begin();
	ratio = numClouds + 1;
	
	if (whichStart < ratio)
	{
		
		if (m_createNextInCloudNum > pPeople->size()-1)
			
			m_createNextInCloudNum = 0;
		RainCloudParticle* rc = (RainCloudParticle*)(pPeople->at(m_createNextInCloudNum));
		rc->addRainDrop(p);
		++m_createNextInCloudNum;

	}
	else
	{
		
		p->pos = ofVec2f(ofRandom(0.f, ofGetWidth()), 0.f);
		p->vel = ofVec2f(0.f, ofRandom(ofGetHeight() * 0.005f, ofGetHeight() * 0.01f));
	whichStart = 0;
	}
	whichStart++;
	//m_createNextInCloud = !m_createNextInCloud;
}

void RainScene::convertPeopleVector()
{
	vector<Particle*> newPeople = vector<Particle*>();

	for (vector<Particle*>::iterator pOld = pPeople->begin(); pOld != pPeople->end(); ++pOld)
	{
		RainCloudParticle* p = new RainCloudParticle((*pOld)->pos);
		p->m_cloudImage = &m_cloudImage;
		p->m_size = ofVec2f(m_cloudImage.getWidth(), m_cloudImage.getHeight());
		newPeople.push_back(p);
	}
	*pPeople = newPeople;
}
Particle* RainScene::addParticleOfProperType(ofVec3f _pos)
{
	RainCloudParticle* p = new RainCloudParticle(_pos);
	p->m_cloudImage = &m_cloudImage;
	p->m_size = ofVec2f(m_cloudImage.getWidth(), m_cloudImage.getHeight());
	pPeople->push_back(p);
	return p;
}

RainScene::~RainScene()
{
}
