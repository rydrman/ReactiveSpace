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
	handImgBase.loadImage("GeoScene/Hand_White.png");
	handImgGreen.loadImage("GeoScene/Hand_Green.png");
	handImgYellow.loadImage("GeoScene/Hand_Yellow.png");
	handImgOrange.loadImage("GeoScene/Hand_Orange.png");
	handImgRed.loadImage("GeoScene/Hand_Red.png");
}


void GeoHands::drawHands()
{
	
	if(Interval ==0){handImgBase.draw(pos.x-185,pos.y-185,0);}
	if(Interval ==1){handImgGreen.draw(pos.x-185,pos.y-185,0);}
	if(Interval ==2){handImgYellow.draw(pos.x-185,pos.y-185,0);}
	if(Interval ==3){handImgOrange.draw(pos.x-185,pos.y-185,0);}
	if(Interval ==4){handImgRed.draw(pos.x-185,pos.y-185,0);}
		
}
void GeoHands::handCountDown(float deltTime , bool lock)
{
	CountTimer = CountTimer + deltTime*16.f;
			
	
			if (CountTimer > 0){
				Interval = 1;
			}		
			if (CountTimer > 1){
				Interval = 2;
			}
			if (CountTimer > 500){
				Interval = 3;
			}
			if (CountTimer > 750){
				Interval = 4;
			}
			if (CountTimer > 1000){
				Interval = 0;
				CountTimer = 0;
			}

	
}


GeoHands::~GeoHands(void)
{
}


