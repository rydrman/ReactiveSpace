#include "GridScene.h"
#include "ofMain.h"

static const float PARTICLE_ANGRY_DIST_SQRD = 40000;

const ofVec2f s_quadVerts[] = {
  ofVec2f( -1.0f, -1.0f ),
  ofVec2f( 1.0f, -1.0f ),
  ofVec2f( 1.0f, 1.0f ),
  ofVec2f( -1.0f, 1.0f )
};

const ofVec2f s_quadTexCoords[] = {
  ofVec2f(0.0f, 0.0f),
  ofVec2f(64.0f, 0.0f),
  ofVec2f(64.0f, 64.0f),
  ofVec2f(0.0f, 64.0f)
};

const ofFloatColor s_quadColors[] = {
  ofFloatColor(1.0, 0.0f, 0.0f, 1.0f),
  ofFloatColor(1.0, 1.0f, 0.0f, 1.0f),
  ofFloatColor(0.0, 0.0f, 1.0f, 1.0f),
  ofFloatColor(0.0, 1.0f, 1.0f, 1.0f)
};

GridScene::GridScene(vector<Particle>* people, vector<Vector4>* hands)
: IScene(people, hands)
{
	//ofDisableArbTex();

	int screenHeight = ofGetScreenHeight();
	int screenWidth = ofGetScreenWidth();
	float scale = screenHeight * screenWidth;
	float particleSize = sqrt(scale) / 60.f;

	m_gridWidth = floor(screenWidth / particleSize);
	m_gridHeight = floor(screenHeight / particleSize);
	m_gridSpacing = (float)screenWidth / (float)m_gridWidth;

	m_particleList = vector<BirdParticle>(m_gridHeight * m_gridWidth);
	m_particleGrid = new BirdParticle*[m_gridWidth];

	// full viewport quad vbo
	m_particleVbo.setVertexData( &s_quadVerts[0], 4, GL_STATIC_DRAW );
	m_particleVbo.setTexCoordData( &s_quadTexCoords[0], 4, GL_STATIC_DRAW );
	m_particleVbo.setColorData( &s_quadColors[0], 4, GL_STATIC_DRAW );

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
	m_particleShader.load("GridScene/gridParticle");
	//particleImage->resize(particleSize * 0.8, particleSize * 0.8);

	glEnable( GL_CULL_FACE );
	glCullFace( GL_FRONT );
}

void GridScene::Render()
{
	//draw grid
	ofSetColor(255);

//		GLint textureId = particleImage->getTextureReference().texData.textureID;
//		glEnable( GL_TEXTURE_2D );
	//	glActiveTexture( GL_TEXTURE0 );
//		glBindTexture( GL_TEXTURE_2D, textureId );

	particleImage->getTextureReference().bind();
	m_particleVbo.bind();
	m_particleShader.begin();

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

		red = ofMap(red, 0, PARTICLE_ANGRY_DIST_SQRD, 0, 1, true);

		m_particleShader.setUniform4f("vColor", 1.f, red, red, 1.f);

		ofPushMatrix();

			ofTranslate(p->pos);
			ofScale(8.0, 8.0);
			int alpha = ofNoise(p->noiseX, p->noiseY) * 150 + 50;
			//ofSetColor(255, red, red, alpha); 


			glDrawArrays( GL_QUADS, 0, 4 );

		ofPopMatrix();

	}
	
	particleImage->getTextureReference().unbind();
	m_particleVbo.unbind();
	m_particleShader.end();

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
