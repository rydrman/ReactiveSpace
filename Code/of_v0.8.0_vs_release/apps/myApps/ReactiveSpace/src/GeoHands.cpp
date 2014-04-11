#include "GeoHands.h"

static float s_countDownTime = 1000;

GeoHands::GeoHands(Particle _old)
:Particle(_old)
{
	CountTimer = 0;
	Interval = 0;
	connected = false;
}


GeoHands::GeoHands(ofVec3f _pos)
:Particle(_pos)
{
	CountTimer = 0;
	Interval = 0;
	connected = false;
}


void GeoHands::drawHands(ofImage* geoHands)
{
	float halfW = geoHands->getWidth() * 0.5f;
	if(Interval ==0){
		ofSetColor(255,255,255,100);
		geoHands->draw(pos.x - halfW, pos.y - halfW);
	}
	if(Interval ==1){
		ofSetColor(130,255,0,70);
		geoHands->draw(pos.x - halfW + ofRandom(-5, 5), pos.y - halfW + ofRandom(-5, 5));
	}
	if(Interval ==2){
		ofSetColor(255,230,0,80);
		geoHands->draw(pos.x - halfW + ofRandom(-10, 10), pos.y - halfW + ofRandom(-10, 10));
	}
	if(Interval ==3){
		ofSetColor(255,17,0,90);
		geoHands->draw(pos.x - halfW + ofRandom(-15, 15), pos.y - halfW + ofRandom(-15, 15));
	}
	if(Interval ==4){
		ofSetColor(255,0,0,100);
		geoHands->draw(pos.x - halfW + ofRandom(-20, 20), pos.y - halfW + ofRandom(-20, 20));
	}
}

void GeoHands::update(float timeScale)
{
	CountTimer += 16.f / timeScale;
			
		if(connected){
			if (CountTimer < s_countDownTime * 0.25f){
				Interval = 1;
			}		
			else if (CountTimer < s_countDownTime * 0.5f){
				Interval = 2;
			}
			else if (CountTimer < s_countDownTime * 0.75f){
				Interval = 3;
			}
			else {
				Interval = 4;
			}
		}else{
				Interval = 0;
				CountTimer = 0;
		}
}


GeoHands::~GeoHands(void)
{
}


