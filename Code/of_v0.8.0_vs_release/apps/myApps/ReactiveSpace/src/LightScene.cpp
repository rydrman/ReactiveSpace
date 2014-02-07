#include "LightScene.h"


LightScene::LightScene(vector<Particle>* people, vector<Vector4>* hands)
: IScene(people, hands)
{
	ofBackground(0, 255);

	pPeople = people;
	
	m_hexImg.loadImage("LightScene/hex.png");

	m_lightImg.loadImage("LightScene/light.png");

	m_lights = vector<light>();

	
	for(int i = 0; i < ofGetWidth(); i += m_lightImg.width){
		light l = light();
		l.isOn = false;
		l.x = i;
		m_lights.push_back(l);	
	}
	m_hexShape = ofPolyline();

	m_hexShape.addVertex(0,0);
	m_hexShape.addVertex(5,5);
	m_hexShape.addVertex(8.5,5);
	m_hexShape.addVertex(11.5,0);
	m_hexShape.addVertex(8.5,-5);
	m_hexShape.addVertex(5,-5);
	m_hexShape.close();
	
}

void LightScene::Render()
{
	//draw lights
	ofPushMatrix();
		ofScale(1.0f,1.0f,1.0f);
		ofSetColor(255);
	
	for(vector<light>::iterator l = m_lights.begin(); l != m_lights.end(); ++l){
		m_lightImg.draw(l->x, 0, 0);
	}

	//draw a hexagon for each person in people
	for(vector<Particle>::iterator p = pPeople->begin(); p != pPeople->end(); ++p)
	{
		ofPushMatrix();
			ofNoFill();
			ofSetLineWidth(3);
			ofTranslate(p->pos);
			m_hexImg.draw(0,0,0);
			//m_hexShape.draw();
		ofPopMatrix();
	}
	for(vector<Particle>::iterator p = pPeople->begin(); p != pPeople->end(); ++p){
		for(vector<light>::iterator l = m_lights.begin(); l != m_lights.end(); ++l){
			m_distance = p->pos.x - l->x;

			if(m_distance <= 0)
				l->isOn = true;
		}
	}
	// I AM ATTEMPTING TO MAKE LIGHT 
	ofLight mylight = ofLight();
	//ofEnableLighting();
	mylight.setPointLight();
	mylight.setSpecularColor(100);
	
	mylight.setGlobalPosition(200,200, 50);

	ofPopMatrix();
}

void LightScene::Update(int deltaTime)
{

}

LightScene::~LightScene()
{

}
