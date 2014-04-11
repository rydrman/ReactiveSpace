#include "RainCloudParticle.h"

RainCloudParticle::RainCloudParticle(ofVec3f _pos)
: Particle(_pos)
{
	m_size = ofVec2f(1.f, 1.f);
	rando = ofRandom(4);
}

void RainCloudParticle::draw()
{

	ofPushMatrix();
		ofTranslate(pos.x, pos.y);
		//m_cloudImage->draw(-m_size.x * 0.5f, -m_size.y * 0.5f );
		
		//ofScale(0.2, 0.2);
		m_cloudImage->drawSubsection(-m_size.x * 0.125f, -m_size.y * 0.5f, m_size.x, m_size.y, m_size.x*rando , 0);
		
		
	ofPopMatrix();
}

void RainCloudParticle::addRainDrop(Particle* p)
{
	p->pos = ofVec3f(pos.x + m_size.x * 0.1f + ofRandom(m_size.x * 0.6f), pos.y, 0.f);
	p->vel = ofVec2f(0.f, ofRandom(ofGetHeight() * 0.005f, ofGetHeight() * 0.01f));
}

RainCloudParticle::~RainCloudParticle(void)
{
}
