#include "GeoHands.h"


GeoHands::GeoHands(Particle _old)
:Particle(_old)
{
}


GeoHands::GeoHands(ofVec3f _pos)
:Particle(_pos)
{
	
	CountTimer = 0;
	Interval = 0;
	
}


void GeoHands::drawHands(ofImage geoHands)
{
	
	if(Interval ==0){
		ofSetColor(255,255,255,100);
		geoHands.draw(pos.x-185,pos.y-185);
	}
	if(Interval ==1){
		ofSetColor(130,255,0,70);
		geoHands.draw(pos.x-185+ofRandom(-5,5),pos.y-185+ofRandom(-5,5));
	}
	if(Interval ==2){
		ofSetColor(255,230,0,80);
		geoHands.draw(pos.x-185+ofRandom(-10,10),pos.y-185+ofRandom(-10,10));
	}
	if(Interval ==3){
		ofSetColor(255,17,0,90);
		geoHands.draw(pos.x-185+ofRandom(-15,15),pos.y-185+ofRandom(-15,15));
	}
	if(Interval ==4){
		ofSetColor(255,0,0,100);
		geoHands.draw(pos.x-185+ofRandom(-20,20),pos.y-185+ofRandom(-20,20));
	}

	
		
}
void GeoHands::handCountDown(float deltTime , bool lock)
{
	CountTimer = CountTimer + deltTime*16.f;
			
		if(lock  == true){
			if (CountTimer > 0){
				Interval = 1;
			}		
			if (CountTimer > 500){
				Interval = 2;
			}
			if (CountTimer > 1000){
				Interval = 3;
			}
			if (CountTimer > 1500){
				Interval = 4;
			}
			if (CountTimer >2000){
				Interval = 0;
				CountTimer = 0;
			}
		}else{
				Interval = 0;
				CountTimer = 0;
		}
			/*if(CountTimer > 3000){
			Interval = 0;
			CountTimer = 0;
			}*/
}


GeoHands::~GeoHands(void)
{
}


