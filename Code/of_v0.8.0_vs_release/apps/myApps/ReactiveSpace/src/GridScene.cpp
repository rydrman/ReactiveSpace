#include "GridScene.h"
#include "ofMain.h"


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
			p.pos.x = i * m_gridSpacing;
			p.pos.y = j * m_gridSpacing;
			p.color = 255;
			p.rad = particleSize * 0.4;
			p.speed = ofPoint(0.f, 0.f);
			p.noiseX = i * j;

			m_particleList.push_back(p);
			m_particleGrid[i][j] = p;
		}
	}
}

void GridScene::Render()
{
	ofBackground(0, 255);

	//draw grid
	/*for (vector<BirdParticle>::iterator p = m_particleList.begin(); p != m_particleList.end(); ++p)
	{
		int col = ofNoise(p->noiseX) * 255;
		ofSetColor(col); 
		ofFill();

		ofCircle(p->pos, p->rad);
	}*/

	//draw hands
	for (vector<Vector4>::iterator h = pHandPositions->begin(); h != pHandPositions->end(); ++h)
	{
		ofSetColor(255, 0, 0, 255);
		ofFill();

		ofPoint pos = ofPoint( h->x * ofGetScreenWidth(), h->y * ofGetScreenHeight() );

		ofCircle(pos, 50);
	}
}

void GridScene::Update(int deltaTime)
{
	for (vector<BirdParticle>::iterator p = m_particleList.begin(); p != m_particleList.end(); ++p)
	{
		p->noiseX += deltaTime / 10000.f;
	}
}

GridScene::~GridScene()
{
}
