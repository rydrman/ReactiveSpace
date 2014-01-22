#include "GridScene.h"
#include "ofMain.h"


GridScene::GridScene(vector<Particle>* people)
: IScene(people)
{
	int screenHeight = ofGetScreenHeight();
	int screenWidth = ofGetScreenWidth();
	float scale = screenHeight * screenWidth;
	float particleSize = sqrt(scale) / 80.f;

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

	//for (vector<Particle>::iterator p = pPeople->begin(); p != pPeople->end(); ++p)
	//{
	//	ofSetColor(255);
	//	ofFill();
	//	ofCircle(p->pos, 10.f);

	//	//create connections
	//	ofPath connections = ofPath();
	//	connections.setStrokeColor(ofColor(255));
	//	connections.setStrokeWidth(3.f);
	//	connections.setMode(ofPath::POLYLINES);
	//	for (vector<Particle>::iterator p2 = pPeople->begin(); p2 != pPeople->end(); ++p2)
	//	{
	//		if (p < p2
	//			&& p->pos.squareDistance(p2->pos) < 80000)
	//		{
	//			connections.moveTo(p->pos);
	//			connections.lineTo(p2->pos);
	//		}
	//	}

	//	//draw connections
	//	connections.draw();
	//}

	//draw grid
	for (vector<BirdParticle>::iterator p = m_particleList.begin(); p != m_particleList.end(); ++p)
	{
		int alpha = ofNoise(p->noiseX) * 255;
		ofSetColor(alpha); 
		ofFill();

		ofCircle(p->pos, p->rad);
	}
}

void GridScene::Update(int deltaTime)
{
	for (vector<BirdParticle>::iterator p = m_particleList.begin(); p != m_particleList.end(); ++p)
	{
		p->noiseX += deltaTime/1000.f;
	}
}

GridScene::~GridScene()
{
}
