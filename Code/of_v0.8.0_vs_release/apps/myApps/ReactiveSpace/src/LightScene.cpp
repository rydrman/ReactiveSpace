#include "LightScene.h"


LightScene::LightScene(vector<Particle*>* people, vector<Vector4>* hands)
: IScene(people, hands)
{
	ofBackground(0, 255);

	pPeople = people;
	
	m_hexImg.loadImage("LightScene/hex.png");
	m_lightImg.loadImage("LightScene/light.png");

	m_lights = vector<Light>();

	
	for(int i = 0; i < ofGetWidth(); i += m_lightImg.width){
		Light l = Light();
		l.isOn = false;
		l.x = i;
		m_lights.push_back(l);	
	}
	
}

void LightScene::Render()
{
	//draw lights
	ofPushMatrix();

	ofScale(1.0f,1.0f,1.0f);
	ofSetColor(255);
	
	for(vector<Light>::iterator l = m_lights.begin(); l != m_lights.end(); ++l){
		if(l->isOn == true){
			m_lightImg.draw(l->x, 0, 0);
		}
	}

	//draw a hexagon for each person in people
	for(vector<Particle*>::iterator p = pPeople->begin(); p != pPeople->end(); ++p)
	{
		ofPushMatrix();
			ofTranslate((*p)->pos);
			ofSetColor(50, ofRandom(50,150), ofRandom(150,255), 200);
			//ofSetColor(hexColor);
			m_hexImg.draw(0,0,0);
		ofPopMatrix();
	}

	ofPopMatrix();
}

void LightScene::Update(int deltaTime)
{
	float distance;
	
	for(vector<Light>::iterator l = m_lights.begin(); l != m_lights.end(); ++l){
		l->isOn = false;
	}

	for(vector<Particle*>::iterator p = pPeople->begin(); p != pPeople->end(); ++p){
		for(vector<Light>::iterator l = m_lights.begin(); l != m_lights.end(); ++l){

			if((*p)->pos.x < l->x+m_lightImg.width && (*p)->pos.x+m_hexImg.width > l->x){
				l->isOn = true;
			}
		}
	}
}

void LightScene::convertPeopleVector()
{
	vector<Particle*> newPeople = vector<Particle*>();

	for (vector<Particle*>::iterator pOld = pPeople->begin(); pOld != pPeople->end(); ++pOld)
	{
		HexagonParticle* p = new HexagonParticle(**pOld._Ptr);
		newPeople.push_back(p);
	}
	*pPeople = newPeople;
}
Particle* LightScene::addParticleOfProperType(ofVec3f _pos)
{
	HexagonParticle* p = new HexagonParticle(_pos);
	pPeople->push_back(p);
	return p;
}

LightScene::~LightScene()
{

}
