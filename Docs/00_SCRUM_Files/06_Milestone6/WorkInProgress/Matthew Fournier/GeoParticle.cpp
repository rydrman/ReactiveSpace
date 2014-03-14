#include "GeoParticle.h"

GeoParticle::GeoParticle(Particle _old)
:Particle(_old)
{
}

GeoParticle::GeoParticle(ofVec3f _pos)
:Particle(_pos)
{	
	startTime = 0;
	
	int whichGeo = ofRandom(1,3);
	GeoSize = ofRandom(1,3);
		if(whichGeo == 1){
			m_shapes.loadImage("GeoScene/Geometric1.png");
		}
		if(whichGeo == 2){
			m_shapes.loadImage("GeoScene/Geometric2.png");
		}
		if(whichGeo == 3){
			m_shapes.loadImage("GeoScene/Geometric3.png");
		}
			
}

void GeoParticle::draw()
{
	ofScale(1/GeoSize,1/GeoSize);
	m_shapes.draw(0,0,0);


}

void GeoParticle::countDown(int dTime)
{
	//if (vel.x == 0){
			startTime = startTime + dTime;
								
			if (startTime > 1000){
				explode();
				startTime = 0;

			}
	//}
}

void GeoParticle::explode()
{
	pos.x = 0;
	pos.y = 0;
	
}


GeoParticle::~GeoParticle(void)
{
}
