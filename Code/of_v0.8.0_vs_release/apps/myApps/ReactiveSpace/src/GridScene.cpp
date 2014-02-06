#include "GridScene.h"
#include "ofMain.h"

static const float PARTICLE_ANGRY_DIST_SQRD = 22500;
static const float PARTICLE_HAPPY_DIST_SQRD = 10000;
static const float PARTICLE_ANGRY_DIST = 150;
static const float PARTICLE_HAPPY_DIST = 100;

const ofVec2f s_quadVerts[] = {
	ofVec2f(-1.0f, -1.0f),
	ofVec2f(1.0f, -1.0f),
	ofVec2f(1.0f, 1.0f),
	ofVec2f(-1.0f, 1.0f)
};

const ofVec2f s_quadTexCoords[] = {
	ofVec2f(0.0f, 0.0f),
	ofVec2f(1.0f, 0.0f),
	ofVec2f(1.0f, 1.0f),
	ofVec2f(0.0f, 1.0f)
};

GridScene::GridScene(vector<Particle>* people, vector<Vector4>* hands)
: IScene(people, hands)
{

	int screenHeight = ofGetScreenHeight();
	int screenWidth = ofGetScreenWidth();
	float scale = screenHeight * screenWidth;
	float particleSize = sqrt(scale) / 50.f;

	particleUpdateOffset = false;

	m_gridWidth = floor(screenWidth / particleSize);
	m_gridHeight = floor(screenHeight / particleSize);
	m_gridSpacing = (float)screenWidth / (float)m_gridWidth;

	m_particleList = vector<BirdParticle>();
	//m_particleGrid = new BirdParticle*[m_gridWidth];

	// full viewport quad vbo
	m_particleVbo.setVertexData( &s_quadVerts[0], 4, GL_STATIC_DRAW );
	m_particleVbo.setTexCoordData( &s_quadTexCoords[0], 4, GL_STATIC_DRAW );

	for (int i = 0; i < m_gridWidth; ++i)
	{
		//m_particleGrid[i] = new BirdParticle[m_gridHeight];
	}

	for (int i = 0; i < m_gridWidth; ++i)
	{
		for (int j = 0; j < m_gridHeight; ++j)
		{
			BirdParticle p = BirdParticle(
				ofVec2f((i + 0.5) * m_gridSpacing, (j + 0.5) * m_gridSpacing),
				m_gridSpacing,
				m_gridSpacing * 0.1
				);

			//set positions
			p.originalPos.x = (i + 0.5) * m_gridSpacing;
			p.originalPos.y = (j + 0.5) * m_gridSpacing;

			//other vars
			p.color = 255;
			p.rad = particleSize * 0.4;
			p.noiseX = i + 0.f;
			p.noiseY = j + 1000.f;
			p.mood = 0.5;

			m_particleList.push_back(p);
			//m_particleGrid[i][j] = p;
		}
	}

	//load resources
	m_particleImage = ofImage();
	m_particleImage.loadImage("GridScene/GridParticle.png");
	m_particleShader = ofShader();
	m_particleShader.load("GridScene/gridParticle");
	//particleImage->resize(particleSize * 0.8, particleSize * 0.8);
}

void GridScene::Render()
{
	//draw grid
	ofSetColor(255);

	//ofDisableArbTex();

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);
	
	m_particleShader.begin();
	m_particleImage.getTextureReference().bind();
	m_particleVbo.bind();
    //m_particleShader.setUniformTexture("tex0", particleImage.getTextureReference(), 0);
	
	
	for (vector<BirdParticle>::iterator p = m_particleList.begin(); p < m_particleList.end(); ++p)
	{
		m_particleShader.setUniform4f("uColor", 
									  1.f - p->mood, 
									  (p->mood > 0.5) ? ofMap(p->mood, 0.5f, 1.f, 0.5f, 0.7f) : 0.5f, 
									  p->mood, 
									  1.f);
		ofPushMatrix();
			ofTranslate(p->pos);
			ofScale(10.0, 10.0);
			//int alpha = ofNoise(p->noiseX, p->noiseY) * 150 + 50;
			glDrawArrays( GL_QUADS, 0, 4 );
		ofPopMatrix();
	}
	
	//draw people
	m_particleShader.setUniform4f("uColor", 1.f, 1.f, 1.f, 1.f);
	for (vector<Particle>::iterator p = pPeople->begin(); p != pPeople->end(); ++p)
	{
		ofPushMatrix();

			ofTranslate(p->pos);
			ofScale(20.0, 20.0);

			glDrawArrays(GL_QUADS, 0, 4);

		ofPopMatrix();
	}

	m_particleImage.getTextureReference().unbind();
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
	int i = 0;
	for (vector<BirdParticle>::iterator p = m_particleList.begin(); p != m_particleList.end(); ++p)
	{
		p->noiseX += deltaTime / 10000.f;
		p->noiseY += deltaTime / 10000.f;

		if ((particleUpdateOffset && i % 2 == 0)
			|| (!particleUpdateOffset && i % 2 != 0))
		{
			//check distance to hands
			bool edit = false;
			float dist;
			for (vector<Vector4>::iterator h = pHandPositions->begin(); h != pHandPositions->end(); ++h)
			{
				dist = ofDistSquared(p->pos.x, p->pos.y, h->x, h->y);
				if (dist < PARTICLE_ANGRY_DIST_SQRD)
				{
					//change mood
					p->mood -= (1.f - dist / PARTICLE_ANGRY_DIST_SQRD) * 0.1f;
					edit = true;

					//push away
					float angle = atan2f(p->pos.y - h->y, p->pos.x - h->x);
					float forceMag = 1.f * (1.f - dist / PARTICLE_ANGRY_DIST_SQRD);
					p->applyForce(cosf(angle) * forceMag, sinf(angle) * forceMag);
				}
			}

			//distance to people
			float happiness = 0;
			for (vector<Particle>::iterator ppl = pPeople->begin(); ppl != pPeople->end(); ++ppl)
			{
				//simple check before distance
				if (p->pos.x > ppl->pos.x + PARTICLE_HAPPY_DIST
					|| p->pos.x < ppl->pos.x - PARTICLE_HAPPY_DIST
					|| p->pos.y > ppl->pos.y + PARTICLE_HAPPY_DIST
					|| p->pos.y < ppl->pos.y - PARTICLE_HAPPY_DIST)
					continue;

				//check distance
				dist = (p->pos.x - ppl->pos.x) * (p->pos.x - ppl->pos.x) + (p->pos.y - ppl->pos.y) * (p->pos.y - ppl->pos.y);
				if (dist < PARTICLE_HAPPY_DIST_SQRD)
				{
					//change mood
					p->mood += (1.f - dist / PARTICLE_HAPPY_DIST_SQRD) * 0.1f;
					edit = true;

					//move away
					//float percDist = dist / PARTICLE_HAPPY_DIST_SQRD;
				}
			}

			//otherwise approach normal
			if (!edit)
			{
				//approach 0.5
				p->mood += (0.5 - p->mood) * 0.1f;

				//approach original position
				p->seek();
			}

			p->mood = ofClamp(p->mood, 0.f, 1.f);
		}



		++i;

		//update it 
		p->update();
	}

	particleUpdateOffset = !particleUpdateOffset;
}

GridScene::~GridScene()
{
}
