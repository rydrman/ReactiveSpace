#include "LightScene.h"

const ofVec2f s_fogTexCoords[] = {
	ofVec2f(0.0f, 0.0f),
	ofVec2f(1.0f, 0.0f),
	ofVec2f(1.0f, 1.0f),
	ofVec2f(0.0f, 1.0f)
};

const ofVec2f s_hexTexCoords[] = {
	ofVec2f(0.0f, 0.0f),
	ofVec2f(1.0f, 0.0f),
	ofVec2f(1.0f, 1.0f),
	ofVec2f(0.0f, 1.0f)
};

LightScene::LightScene(vector<Particle*>* people, vector<Particle*>* hands)
: IScene(people, hands)
{
	const ofVec2f fogVerts[] = {
		ofVec2f(0.f, 0.f),
		ofVec2f(ofGetWidth(), 0.f),
		ofVec2f(ofGetWidth(), ofGetHeight()),
		ofVec2f(0.0f, ofGetHeight())
	};

	pPeople = people;
	pHandPositions = hands;
	
	//load all images 
	m_hexImg.loadImage("LightScene/hex.png");
	m_lightImg.loadImage("LightScene/light.png");
	m_fogImg.loadImage("LightScene/fog.png");

	const ofVec2f hexVerts[] = {
		ofVec2f(-m_hexImg.width, -m_hexImg.height),
		ofVec2f(m_hexImg.width, -m_hexImg.height),
		ofVec2f(m_hexImg.width, m_hexImg.height),
		ofVec2f(-m_hexImg.width, m_hexImg.height)
	};



	m_lights = vector<Light>();

	
	for(int i = 0; i < ofGetWidth(); i += m_lightImg.width){
		Light l = Light();
		l.isOn = false;
		l.x = i;
		m_lights.push_back(l);	
	}

	//create an image that holds is the length of the amount of lights. set pixel to white if light is on
	m_lightsOnTexture.allocate(m_lights.size(), 1, GL_LUMINANCE);
	m_lightsOnTexture.clear();
	m_lightsOnTexture.readToPixels(m_lightsOnPixels);

	//setting up shader for fog
	m_fogShader = ofShader();
	m_fogShader.load("LightScene/fog");
	m_fogVbo = ofVbo();
	m_fogVbo.setVertexData( &fogVerts[0], 4, GL_STATIC_DRAW);
	m_fogVbo.setTexCoordData(&s_fogTexCoords[0], 4, GL_STATIC_DRAW);
	m_fogInt = 0;

	//setting up hexagon shader
	m_hexShader = ofShader();
	m_hexShader.load("LightScene/hex");
	m_hexVbo = ofVbo();
	m_hexVbo.setVertexData( &hexVerts[0], 4, GL_STATIC_DRAW);
	m_hexVbo.setTexCoordData(&s_hexTexCoords[0], 4, GL_STATIC_DRAW);

}

void LightScene::Render()
{
	//FOG 
	m_fogShader.begin();
	m_fogImg.getTextureReference().bind();
	m_fogVbo.bind();

	//draw fog shader
	m_fogShader.setUniformTexture("lightsOnPix", m_lightsOnTexture, 1);
	m_fogShader.setUniform1f("numLights", m_lights.size());
	glDrawArrays(GL_QUADS, 0, 4);

	m_fogImg.getTextureReference().unbind();
	m_fogShader.end();
	m_fogVbo.unbind();
	

	//draw lights
	ofPushMatrix();
	ofScale(1.0f,1.0f,1.0f);
	ofSetColor(255);

	for(vector<Light>::iterator l = m_lights.begin(); l != m_lights.end(); ++l){
		if(l->isOn == true){
			m_lightImg.draw(l->x, 0, 0);
		}
	}

	//HEXAGON
	m_hexShader.begin();
	m_hexImg.getTextureReference().bind();
	m_hexVbo.bind();

	//draw hex shader
	m_hexShader.setUniform4f("hexColor", 0, 0, 0, 0);

	//draw a hexagon for each person in people
	for(vector<Particle*>::iterator p = pPeople->begin(); p != pPeople->end(); ++p)
	{
		ofPushMatrix();
			ofTranslate((*p)->pos);
			//ofSetColor(50, ofRandom(50,150), ofRandom(150,255), 200);
			//ofSetColor(hexColor);
			glDrawArrays(GL_QUADS, 0, 4);
		ofPopMatrix();
	}

	ofPopMatrix();

	m_hexImg.getTextureReference().unbind();
	m_hexShader.end();
	m_hexVbo.unbind();
	
}

void LightScene::Update(int deltaTime)
{
	float distance;
	m_fogInt = 0;
	int count = 0; 

	
	for(vector<Light>::iterator l = m_lights.begin(); l != m_lights.end(); ++l){
		l->isOn = false;
		m_lightsOnPixels.setColor(count+1, 1, ofColor(0));
		count++;
	}

	count=0;
	for(vector<Particle*>::iterator p = pPeople->begin(); p != pPeople->end(); ++p){
		for(vector<Light>::iterator l = m_lights.begin(); l != m_lights.end(); ++l){

			if((*p)->pos.x < l->x+m_lightImg.width && (*p)->pos.x+m_hexImg.width > l->x){
				l->isOn = true;
				m_lightsOnPixels.setColor(count+1, 1, ofColor(255));
			}
			count++;
		}
	}
	m_lightsOnTexture.loadData(m_lightsOnPixels, GL_LUMINANCE);
	
//	for(vector<IScene>::iterator hands = pHandPositions->begin(); hands != pHandPositions->end(); ++hands){
		
	//}
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
