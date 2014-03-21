#include "RainCloudParticle.h"

RainCloudParticle::RainCloudParticle(ofVec3f _pos)
: Particle(_pos)
{
	clouds.loadImage("RainScene/cloud.png");

	//CloudSize = ofRandom(ofGetWidth() * 0.05, ofGetWidth() * 0.1);
}

void RainCloudParticle::draw()
{
	ofPushMatrix();
	ofTranslate(pos.x, pos.y);
	//ofScale(CloudSize, CloudSize);
	ofPopMatrix();
}

RainCloudParticle::~RainCloudParticle(void)
{
}
