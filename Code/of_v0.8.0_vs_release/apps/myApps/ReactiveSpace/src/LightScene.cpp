#include "LightScene.h"

/*const ofVec2f s_fogTexCoords[] = {
	ofVec2f(0.0f, 0.0f),
	ofVec2f(1.0f, 0.0f),
	ofVec2f(1.0f, 1.0f),
	ofVec2f(0.0f, 1.0f)
};*/

LightScene::LightScene(vector<Particle*>* people, vector<Particle*>* hands)
: IScene(people, hands)
{
	/*const ofVec2f fogVerts[] = {
		ofVec2f(0.f, 0.f),
		ofVec2f(0.f, ofGetWidth()),
		ofVec2f(ofGetWidth(), ofGetHeight()),
		ofVec2f(0.0f, ofGetHeight())
	};*/

	pPeople = people;
	
	//load all images 
	m_hexImg.loadImage("LightScene/hex.png");
	m_lightImg.loadImage("LightScene/light.png");
	m_fogImg.loadImage("LightScene/fog.png");

	m_lights = vector<Light>();

	
	for(int i = 0; i < ofGetWidth(); i += m_lightImg.width){
		Light l = Light();
		l.isOn = false;
		l.x = i;
		m_lights.push_back(l);	
	}
	
	//setting up shader for fog
	//m_fogShader = ofShader();
	//m_fogShader.load("LightScene/fog");
	//m_fogVbo = ofVbo();
	//m_fogVbo.setVertexData( &fogVerts[0], 4, GL_STATIC_DRAW);
	//m_fogVbo.setTexCoordData(&s_fogTexCoords[0], 4, GL_STATIC_DRAW);
	//m_fogInt = 0;

}

void LightScene::Render()
{

	//m_fogShader.begin();
	//m_fogImg.getTextureReference().bind();
	//m_fogVbo.bind();

	//draw fog
//	m_fogShader.setUniform1i("lightsOnBits", m_fogInt);
//	m_fogShader.setUniform1f("numLights", m_lights.size());
//	glDrawArrays(GL_QUADS, 0, 4);

	//m_fogImg.getTextureReference().unbind();
	//m_fogShader.end();
	//m_fogVbo.unbind();

	m_fogImg.draw(0, 0, 10);

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
	m_fogInt = 0;
	int count = 0; 

	
	for(vector<Light>::iterator l = m_lights.begin(); l != m_lights.end(); ++l){
		l->isOn = false;
	}

	for(vector<Particle*>::iterator p = pPeople->begin(); p != pPeople->end(); ++p){
		for(vector<Light>::iterator l = m_lights.begin(); l != m_lights.end(); ++l){

			if((*p)->pos.x < l->x+m_lightImg.width && (*p)->pos.x+m_hexImg.width > l->x){
				l->isOn = true;
				m_fogInt += pow(2, count);

			}
			count++;
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
