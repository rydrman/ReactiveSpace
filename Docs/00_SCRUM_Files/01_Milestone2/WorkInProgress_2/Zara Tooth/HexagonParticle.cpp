#include "HexagonParticle.h"

HexagonParticle::HexagonParticle(ofVec2f _pos)
: Particle(_pos)
{
	hexColor = ofColor(50, ofRandom(50,150), ofRandom(150,255), 200);
}

HexagonParticle::~HexagonParticle(){
	
}