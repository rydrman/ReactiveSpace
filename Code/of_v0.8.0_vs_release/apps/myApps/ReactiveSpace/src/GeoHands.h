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
	bool connected;

	void drawHands(ofImage geoHands);
	void update(float timeScale);
	GeoHands(void);
	~GeoHands(void);
};

