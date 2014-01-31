#include "GridScene.h"
#include "ofMain.h"

static const float PARTICLE_ANGRY_DIST_SQRD = 40000;

GridScene::GridScene(vector<Particle>* people, vector<Vector4>* hands)
: IScene(people, hands)
{
	int screenHeight = ofGetScreenHeight();
	int screenWidth = ofGetScreenWidth();
	float scale = screenHeight * screenWidth;
	float particleSize = sqrt(scale) / 60.f;

	m_gridWidth = floor(screenWidth / particleSize);
	m_gridHeight = floor(screenHeight / particleSize);
	m_gridSpacing = (float)screenWidth / (float)m_gridWidth;

	m_particleList = vector<BirdParticle>(m_gridHeight * m_gridWidth);
	m_particleGrid = new BirdParticle*[m_gridWidth];

	for (int i = 0; i < m_gridWidth; ++i)
	{
		m_particleGrid[i] = new BirdParticle[m_gridHeight];
	}

	for (int i = 0; i < m_gridWidth; ++i)
	{
		for (int j = 0; j < m_gridHeight; ++j)
		{
			BirdParticle p = BirdParticle();
			p.pos.x = (i + 0.5) * m_gridSpacing;
			p.pos.y = (j + 0.5) * m_gridSpacing;
			p.color = 255;
			p.rad = particleSize * 0.4;
			p.vel = ofVec2f(0.f, 0.f);
			p.noiseX = i;
			p.noiseY = j;

			m_particleList.push_back(p);
			m_particleGrid[i][j] = p;
		}
	}

	//load resources
	particleImage = new ofImage();
	particleImage->loadImage("GridScene/GridParticle.png");
	particleImage->resize(particleSize * 0.8, particleSize * 0.8);
}

void GridScene::Render()
{
	//draw grid
	ofSetColor(255);
	for (vector<BirdParticle>::iterator p = m_particleList.begin(); p != m_particleList.end(); ++p)
	{
		//check distance to hands
		ofPoint pos;
		float dist;
		float red = PARTICLE_ANGRY_DIST_SQRD;
		for (vector<Vector4>::iterator h = pHandPositions->begin(); h != pHandPositions->end(); ++h)
		{
			dist = ofDistSquared(p->pos.x, p->pos.y, h->x, h->y);
			red = (dist < red) ? dist : red;
		}

		red = ofMap(red, 0, PARTICLE_ANGRY_DIST_SQRD, 0, 255, true);

		ofPushMatrix();
			ofTranslate(p->pos);
			int alpha = ofNoise(p->noiseX, p->noiseY) * 150 + 50;
			ofSetColor(255, red, red, alpha); 
			particleImage->draw(-p->rad, -p->rad);
		ofPopMatrix();
	}

	//draw hands
	ofSetColor(255, 0, 0, 255);
	ofFill();
	ofPoint pos;
	for (vector<Vector4>::iterator h = pHandPositions->begin(); h != pHandPositions->end(); ++h)
	{
		pos = ofPoint( h->x , h->y );
		ofCircle(pos, 20);
	}
}

void GridScene::Update(int deltaTime)
{
	for (vector<BirdParticle>::iterator p = m_particleList.begin(); p != m_particleList.end(); ++p)
	{
		p->noiseX += deltaTime / 10000.f;
		p->noiseY += deltaTime / 10000.f;
	}
}

GridScene::~GridScene()
{
}
