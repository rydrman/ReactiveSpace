#include "GridScene.h"
#include "ofMain.h"

#define DEBUG_DRAW 1

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

GridScene::GridScene(vector<Particle*>* people, vector<Particle*>* hands)
: IScene(people, hands)
{

	int screenHeight = ofGetHeight();
	int screenWidth = ofGetWidth();
	float scale = screenHeight * screenWidth;
	float particleSize = sqrt(scale) / 30.f;
	m_particleSize = particleSize * 0.4;

	particleUpdateOffset = false;

	m_gridWidth = floor(screenWidth / particleSize);
	m_gridHeight = floor(screenHeight / particleSize);
	m_gridSpacing = (float)screenWidth / (float)m_gridWidth;
	m_numParticles = m_gridHeight * m_gridWidth;

	m_particleList = new BirdParticle [m_numParticles];
	m_angryParticles = vector<BirdParticle*>();

	// full viewport quad vbo
	m_particleVbo.setVertexData( &s_quadVerts[0], 4, GL_STATIC_DRAW );
	m_particleVbo.setTexCoordData( &s_quadTexCoords[0], 4, GL_STATIC_DRAW );

	int count = 0;
	for (int i = 0; i < m_gridWidth; ++i)
	{
		for (int j = 0; j < m_gridHeight; ++j)
		{
			BirdParticle p = BirdParticle(
				ofVec2f((i + 0.5) * m_gridSpacing, (j + 0.5) * m_gridSpacing),
				m_gridSpacing * 0.5,
				m_gridSpacing * 0.25
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

			m_particleList[count] = p;
			++count;
		}
	}

	//load resources
	m_backgroundImage = ofImage();
	m_backgroundImage.loadImage("GridScene/GridBackground.png");
	m_particleImage = ofImage();
	m_particleImage.loadImage("GridScene/GridParticle.png");
	m_particleShader = ofShader();
	m_particleShader.load("GridScene/gridParticle");
}

void GridScene::Render()
{
	ofSetColor(255);
	m_backgroundImage.draw(0.f, 0.f, 0.f, ofGetWidth(), ofGetHeight());

	m_particleShader.begin();
	m_particleImage.getTextureReference().bind();
	m_particleVbo.bind();
	
	BirdParticle* p;
	float alpha;
	for (int i = 0; i < m_numParticles; ++i)
	{
		p = &m_particleList[i];

		//only draw home particles
		if (p->isHome)
		{
			alpha = ofNoise(p->noiseX * 0.08f, p->noiseY * 0.08f) * 0.5f + 0.5f;

			m_particleShader.setUniform4f("uColor",
				1.f - p->mood,
				(p->mood > 0.5) ? ofMap(p->mood, 0.5f, 1.f, 0.5f, 0.7f) : p->mood,
				p->mood,
				alpha);

			ofPushMatrix();
				ofTranslate(p->pos);
				ofScale(m_particleSize, m_particleSize);
				glDrawArrays(GL_QUADS, 0, 4);
			ofPopMatrix();
		}
	}

	//now draw angry particles on top
	alpha = 1.f;
	for (vector<BirdParticle*>::iterator p = m_angryParticles.begin(); p != m_angryParticles.end(); ++p)
	{
		m_particleShader.setUniform4f("uColor",
			1.f -(*p)->mood,
			((*p)->mood > 0.5) ? ofMap((*p)->mood, 0.5f, 1.f, 0.5f, 0.7f) :(*p)->mood,
			(*p)->mood,
			alpha);

		ofPushMatrix();
			ofTranslate((*p)->pos);
			ofScale(m_particleSize, m_particleSize);
			glDrawArrays(GL_QUADS, 0, 4);
		ofPopMatrix();
	}
	
	//draw people
	m_particleShader.setUniform4f("uColor", 1.f, 1.f, 1.f, 1.f);
	for (vector<Particle*>::iterator p = pPeople->begin(); p != pPeople->end(); ++p)
	{
		ofPushMatrix();
			ofTranslate((*p)->pos);
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
	for (vector<Particle*>::iterator h = pHandPositions->begin(); h != pHandPositions->end(); ++h)
	{
		ofPushMatrix();
		ofTranslate((*h)->pos);
		ofCircle(0.f, 0.f, 0.f, 20.f);

#ifdef DEBUG_DRAW
		ofSetColor(0, 0, 0, 255);
		ofFill();
		ofDrawBitmapString(ofToString((*h)->ID, 0), 0.f, 0.f);
		ofSetColor(255, 0, 0, 255);
#endif
		ofPopMatrix();
	}
}

void GridScene::Update(int deltaTime)
{
	int i = 0;
	BirdParticle* p;
	for (int i = (particleUpdateOffset) ? 0 : 1; i < m_numParticles; i+=2)
	{
		p = &m_particleList[i];

		//update noise
		p->noiseX += deltaTime / 200.f;
		p->noiseY += deltaTime / 300.f;

		//check distance to hands
		bool edit = false;
		float dist;
		for (vector<Particle*>::iterator h = pHandPositions->begin(); h != pHandPositions->end(); ++h)
		{
			dist = ofDistSquared(p->pos.x, p->pos.y, (*h)->pos.x, (*h)->pos.y);
			if (dist < PARTICLE_ANGRY_DIST_SQRD)
			{

				//change mood
				p->mood -= (1.f - dist / PARTICLE_ANGRY_DIST_SQRD) * 0.1f;
				edit = true;

				//push away
				float angle = atan2f(p->pos.y - (*h)->pos.y, p->pos.x - (*h)->pos.x);
				float forceMag = 1.f * (1.f - dist / PARTICLE_ANGRY_DIST_SQRD);
				p->applyForce(cosf(angle) * forceMag, sinf(angle) * forceMag);

				//register as away
				if (p->isHome)
				{
					p->isHome = false;
					m_angryParticles.push_back(p);
				}
			}
		}

		//distance to people
		float happiness = 0;
		for (vector<Particle*>::iterator ppl = pPeople->begin(); ppl != pPeople->end(); ++ppl)
		{
			//simple check before distance
			if (p->pos.x > ppl[0]->pos.x + PARTICLE_HAPPY_DIST
				|| p->pos.x < ppl[0]->pos.x - PARTICLE_HAPPY_DIST
				|| p->pos.y > ppl[0]->pos.y + PARTICLE_HAPPY_DIST
				|| p->pos.y < ppl[0]->pos.y - PARTICLE_HAPPY_DIST)
				continue;

			//check distance
			dist = (p->pos.x - ppl[0]->pos.x) * (p->pos.x - ppl[0]->pos.x) + (p->pos.y - ppl[0]->pos.y) * (p->pos.y - ppl[0]->pos.y);
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
			p->mood += (0.5 - p->mood) * 0.01f;
		}

		//approach original position
		if (!p->isHome && p->mood >= 0.45f)
		{
			bool gotHome = p->seek();
			if (gotHome)
			{
				vector<BirdParticle*>::iterator it = std::find(m_angryParticles.begin(), m_angryParticles.end(), (BirdParticle*)&*p);
				if (it != m_angryParticles.end())
					m_angryParticles.erase(it);
			}
		}

		p->mood = ofClamp(p->mood, 0.f, 1.f);

		//update it 
		if (!p->isHome)			
			p->update(&m_angryParticles);

		//reverse speed if at edge of screen
		int screenW = ofGetWidth();
		int screenH = ofGetHeight();
		if (p->pos.x > screenW)
		{
			p->pos.x = screenW;
			p->vel.x *= -1;
		}
		else if (p->pos.x < 0)
		{
			p->pos.x = 0;
			p->vel.x *= -1;
		}
		if (p->pos.y > screenH)
		{
			p->pos.y = screenH;
			p->vel.y *= -1;
		}
		else if (p->pos.y < 0)
		{
			p->pos.y = 0;
			p->vel.y *= -1;
		}
	}

	particleUpdateOffset = !particleUpdateOffset;
}

GridScene::~GridScene()
{
}
