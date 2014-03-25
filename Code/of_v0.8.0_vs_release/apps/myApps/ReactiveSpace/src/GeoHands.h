#pragma once
#include "particle.h"
class GeoHands :
	public Particle
{
public:
	GeoHands(Particle _old);
	GeoHands(ofVec3f _pos);

	ofImage handImgBase;
	ofImage handImgGreen;
	ofImage handImgYellow;
	ofImage handImgOrange;
	ofImage handImgRed;
	
	
	int CountTimer;
	int Interval;
	void drawHands(ofImage geoHands);
	void handCountDown(float timeScale, bool lock);
	GeoHands(void);
	~GeoHands(void);
};

