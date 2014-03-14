#include "HexagonParticle.h"

HexagonParticle::HexagonParticle(Particle _old)
{
	HexagonParticle::HexagonParticle(_old.pos);
}

HexagonParticle::HexagonParticle(ofVec3f _pos)
: Particle(_pos)
{
	hexAlpha = 0; 
	hexColor = ofColor(ofRandom(0,255), ofRandom(0,255), ofRandom(0,255), hexAlpha);
	hexRotation = (int)ofRandom(0,360);
	hexSize = 0.1;

}

HexagonParticle::~HexagonParticle(){
	
}