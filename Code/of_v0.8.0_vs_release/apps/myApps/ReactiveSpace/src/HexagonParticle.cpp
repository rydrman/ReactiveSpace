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
	hexSize = 0.3;
	
	hexGrowthRate = ofRandom(0.005, 0.5);
	hexMaxSize = ofRandom(2,5);
	hexToHands.set(0, 0);

}

HexagonParticle::~HexagonParticle(){
	
}