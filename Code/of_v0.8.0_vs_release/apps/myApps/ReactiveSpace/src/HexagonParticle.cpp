#include "HexagonParticle.h"

HexagonParticle::HexagonParticle(ofVec3f _pos)
:Particle(_pos)
{

	hexAlpha = 0; 
	hexColor = ofColor(ofRandom(0,255), ofRandom(0,255), ofRandom(0,255), hexAlpha);
	hexRotation = (int)ofRandom(0,360);
	hexSize = 0.3;
	
	hexGrowthRate = ofRandom(0.005, 0.5);
	hexMaxSize = ofRandom(2,5);
	hexToHands.set(0, 0);

}

void HexagonParticle::separation(vector<HexagonParticle*>* connectedParticles)
{
	ofVec2f steerSep = ofVec2f(0.f, 0.f);

	int count = 0;
	for (vector<HexagonParticle*>::iterator c = connectedParticles->begin(); c != connectedParticles->end(); ++c){
		float distSqrd = ofDistSquared((*c)->pos.x, (*c)->pos.y, pos.x, pos.y);

		if (distSqrd < s_neighDistSqrd){
		
			if (distSqrd < s_desiredSepSqrd && distSqrd >0){
				steerSep += (pos - (*c)->pos).normalize() / sqrt(distSqrd);
			}
		
			++count;
		}

		if (count > 0){
		
			steerSep /= (float)count;
			steerSep = steerSep.normalize() *maxSpeed;
			steerSep -= vel;
			steerSep.limit(maxForce*0.2);
			accel += steerSep;
			accel.limit(maxSpeed);
		}
	}

}


HexagonParticle::~HexagonParticle(){
	
}