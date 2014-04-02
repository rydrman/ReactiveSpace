#pragma once
#include "ofMain.h"

class Light
{
private:
	bool m_isOn;
	bool m_isPower;
	float timer;

public:
	float x;
	
	Light(void);

	void turnOn();
	bool isOn();
	void turnOff();
	int flickerLight;

	void update(float timeScale);

	~Light(void);

};
