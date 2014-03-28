#include "GeoScene.h"

static int s_radius = 100;
static int s_numConfetti = 1000;
GeoScene::GeoScene(vector<Particle*>* people, vector<Particle*>* hands, AudioManager* audioManager)
: IScene(people, hands, audioManager)
{
	screenW = ofGetWidth();
	screenH = ofGetHeight();	

	geoBack.loadImage("GeoScene/Geometric_Background.jpg");
	geoHands.loadImage("GeoScene/Hand_White.png");
	m_gradients.loadImage("GeoScene/gradients.png");
	explosionSprite.loadImage("GeoScene/Particles2.png");
	m_confettiImg.loadImage("GeoScene/Particles.png");

	geoBackSound = pAudioManager->load("GeoScene/Scene3Back.mp3");
	geoBackSound->setLoop(true); 
	
	geoExplosionSound = pAudioManager->load("GeoScene/Scene3_ExplodeOnly.mp3");
	geoExplosionSound2 = pAudioManager->load("GeoScene/Scene3_ExplodeOnly2.mp3");
	
	m_confetti = new Confetti[s_numConfetti];
	for (int i = 0; i < s_numConfetti; ++i)
	{
		m_confetti[i].p = Particle();
		m_confetti[i].timeout = 0;
		m_confetti[i].spriteX = (int)ofRandom(10);
		m_confetti[i].spriteY = (int)ofRandom(10);
	}
}

void GeoScene::Render()
{
	ofBackground(0, 255);
	ofSetColor(255);
	ofPushMatrix();
		geoBack.draw(0,0,0);
	ofPopMatrix();

	//draw confetti
	Confetti* c;
	for (int i = 0; i < s_numConfetti; ++i)
	{
		c = &m_confetti[i];
		if (c->timeout > 0)
		{
			ofPushMatrix();
				ofSetColor(c->color);
				ofTranslate(c->p.pos);
				m_confettiImg.drawSubsection(0.f, 0.f, 10.f, 10.f, c->spriteX * 10, c->spriteY * 10);
			ofPopMatrix();
		}
	}

	GeoParticle* gp;

	for (vector<Particle*>::iterator p = pPeople->begin(); p != pPeople->end(); ++p)
	{
		gp = (GeoParticle*)(*p);
		gp->draw(&m_gradients, &explosionSprite);
	}

	GeoHands* gh;
	for (vector<Particle*>::iterator h = pHandPositions->begin(); h != pHandPositions->end(); ++h)
	{
		gh = (GeoHands*)(*h);
		gh->drawHands(geoHands);
	}

	
}

void GeoScene::Update(float timeScale)
{
	//update hands
	GeoParticle* gp;
	GeoHands* gh;
	//reset vars first
	for (vector<Particle*>::iterator h = pHandPositions->begin(); h != pHandPositions->end(); ++h)
	{
		((GeoHands*)(*h))->connected = false;
	}
	for (vector<Particle*>::iterator p = pPeople->begin(); p != pPeople->end(); ++p)
	{
		((GeoParticle*)(*p))->connected = false;
	}

	for (vector<Particle*>::iterator h = pHandPositions->begin(); h != pHandPositions->end(); ++h)
	{
		gh= (GeoHands*)(*h);

		for (vector<Particle*>::iterator p = pPeople->begin(); p != pPeople->end(); ++p)
		{
			gp = (GeoParticle*)(*p);
			if (gp->m_isExploding)
			{
				continue;
			}

			if ((*h)->pos.x + s_radius > ((*p)->pos.x) && (*h)->pos.x - s_radius<((*p)->pos.x)
				&& (*h)->pos.y - 50 + s_radius >((*p)->pos.y) && (*h)->pos.y - s_radius < ((*p)->pos.y))
			{
				gp->connected = true;
				gh->connected = true;	

				break;
			}
		}
	}

	//update them
	for (vector<Particle*>::iterator h = pHandPositions->begin(); h != pHandPositions->end(); ++h)
	{
		((GeoHands*)(*h))->update(timeScale);
	}
	for (vector<Particle*>::iterator p = pPeople->begin(); p != pPeople->end();)
	{
		gp = (GeoParticle*)(*p);

		bool wasExploding = gp->m_isExploding;

		gp->update(timeScale);

		if (gp->m_isExploding && !wasExploding)
		{
			//add confetti
			//find unused ones
			int used = 0;
			for (int i = 0; i < s_numConfetti; ++i)
			{
				if (m_confetti[i].timeout <= 0)
				{
					Confetti* c = &m_confetti[i];
					c->p.pos = gp->pos;
					c->p.vel = ofVec2f(ofRandom(-3, 3), ofRandom(-3, 3));
					c->p.restitution = ofRandom(0.95, 0.99);
					c->timeout = ofRandom(100.f, 200.f);
					c->color = gp->getColor();
					++used;
				}

				if (used == 100) break;
			}
		}
		//remove it if necessary
		else if (gp->m_isDead){
			p = pPeople->erase(p);
			continue;
		}
		++p;
	}

	//update confetti
	float unix = (float)ofGetUnixTime();
	for (int i = 0; i < s_numConfetti; ++i)
	{
		if (m_confetti[i].timeout > 0)
		{
			m_confetti[i].timeout -= 0.2f * timeScale;
			m_confetti[i].p.update();
		}
	}
}


void GeoScene::convertPeopleVector()
{
	vector<Particle*> newPeople = vector<Particle*>();

	for (vector<Particle*>::iterator pOld = pPeople->begin(); pOld != pPeople->end(); ++pOld)
	{
		GeoParticle* p;
		//toss a coin to pick sound
		if (ofRandom(1.f) > 0.5f)
		{
			p = new GeoParticle((*pOld)->pos, geoExplosionSound);
		}
		else
		{
			p = new GeoParticle((*pOld)->pos, geoExplosionSound2);
		}

		
		newPeople.push_back(p);
	}
	*pPeople = newPeople;
}
Particle* GeoScene::addParticleOfProperType(ofVec3f _pos)
{
	GeoParticle* p;
	//toss a coin to pick sound
	if (ofRandom(1.f) > 0.5f)
	{
		p = new GeoParticle(_pos, geoExplosionSound);
	}
	else
	{
		p = new GeoParticle(_pos, geoExplosionSound2);
	}
	pPeople->push_back(p);
	return p;
}

void GeoScene::convertHandVector()
{
	vector<Particle*> newHands = vector<Particle*>();

	for (vector<Particle*>::iterator hOld = pHandPositions->begin(); hOld != pHandPositions->end(); ++hOld)
	{
		GeoHands* h = new GeoHands((*hOld)->pos);
		newHands.push_back(h);
	}
	*pHandPositions = newHands;
}
Particle* GeoScene::addHandOfProperType(ofVec3f _pos)
{
	GeoHands* h = new GeoHands(_pos);
	pHandPositions->push_back(h);
	return h;
}

GeoScene::~GeoScene()
{
}

void GeoScene::onLoad()
{
	geoBackSound->play();
}

void GeoScene::onUnload()
{
	geoBackSound->stop();
}
