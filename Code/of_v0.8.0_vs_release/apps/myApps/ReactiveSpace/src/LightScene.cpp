#include "LightScene.h"


LightScene::LightScene(vector<Particle>* people, vector<Vector4>* hands)
: IScene(people, hands)
{
	ofBackground(0, 255);

	Particle hexParticle = Particle();
}

void LightScene::Render()
{
	//I MADE A BOX
	ofPushMatrix();
		ofNoFill();
		ofSetLineWidth(3);
		ofTranslate(500,500,10);
		ofBox(100,100,100);
	ofPopMatrix();

	ofPushMatrix();
	ofTranslate(800, 800, 50);
	ofScale(10,10);
		ofBeginShape();
		ofSetColor(100,100,255);
			ofVertex(0,0,0);
			ofVertex(5,5,0);
			ofVertex(12,5,0);
			ofVertex(18,0,0);
			ofVertex(12,-5,0);
			ofVertex(5,-5,0);
			ofVertex(0,0,0);
		ofEndShape();
	ofPopMatrix();

	// I AM ATTEMPTING TO MAKE LIGHT 
	ofLight mylight = ofLight();
	ofEnableLighting();
	mylight.setPointLight();
	mylight.setSpecularColor(100);
	
	mylight.setGlobalPosition(200,200, 50);

}

void LightScene::Update(int deltaTime)
{

}

LightScene::~LightScene()
{

}
