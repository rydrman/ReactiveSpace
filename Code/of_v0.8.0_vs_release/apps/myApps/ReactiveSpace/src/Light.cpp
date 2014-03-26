#include "Light.h"

int s_flickerMapLen = 3;
int s_flickerTimeMS = 500;
ofVec2f s_flickerMap[] = {
	ofVec2f(0, 50),
	ofVec2f(100, 120),
	ofVec2f(300, 375)
	//ofVec2f(400, 500),
	//ofVec2f(600, 750)
};

Light::Light(void)
{
	m_isOn = false;
	m_isPower = false;
}

void Light::turnOn()
{
	m_isPower = true;
}

bool Light::isOn()
{
	return m_isOn;
}

void Light::turnOff()
{
	m_isPower = false;
	m_isOn = false;
	timer = 0;
}

void Light::update(float timeScale)
{
	if (m_isPower)
	{
		timer += (16.f / timeScale);

		if (timer < s_flickerTimeMS)
		{
			m_isOn = false;
			for (int i = 0; i < s_flickerMapLen; ++i)
			{
				if (timer > s_flickerMap[i].x && timer < s_flickerMap[i].y)
				{
					m_isOn = true;
					break;
				}
			}
		}
		else
		{
			m_isOn = true;
		}
	}
}

Light::~Light(void)
{

}
