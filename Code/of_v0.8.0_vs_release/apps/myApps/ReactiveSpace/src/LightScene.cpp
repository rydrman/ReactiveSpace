#include "LightScene.h"

const ofVec2f s_fogTexCoords[] = {
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
	m_hexImgBorder.loadImage("LightScene/Hexagon.png");
	m_hexImgInner.loadImage("LightScene/hexagonFill.png");
	m_lightImg.loadImage("LightScene/light.png");
	m_fogImg.loadImage("LightScene/fog.png");

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
	ofSetColor(255);
	ofSetRectMode(OF_RECTMODE_CORNER);
	for(vector<Light>::iterator l = m_lights.begin(); l != m_lights.end(); ++l){
		if(l->isOn == true){
			m_lightImg.draw(l->x, 0, 0);
		}
	}


	HexagonParticle* hp;
	//draw a hexagon for each person in people
	for(vector<Particle*>::iterator p = pPeople->begin(); p != pPeople->end(); ++p)
	{
		hp = (HexagonParticle*) (*p);
		ofPushMatrix();
			ofTranslate(hp->pos);
			ofSetRectMode(OF_RECTMODE_CENTER);
			ofRotate(hp->hexRotation);
			ofScale(hp->hexSize, hp->hexSize);

			ofSetColor(hp->hexColor);
			m_hexImgInner.draw(0,0);

			ofSetColor(255);
			m_hexImgBorder.draw(0,0);
		ofPopMatrix();
		
		ofSetRectMode(OF_RECTMODE_CORNER);

		ofNoFill();
		ofSetLineWidth(5);
		float distSqrd = std::numeric_limits<float>::max();

		for(vector<Particle*>::iterator hands = pHandPositions->begin(); hands != pHandPositions->end(); ++hands){
			distSqrd = ofDistSquared( (*hands)->pos.x, (*hands)->pos.y, (*p)->pos.x, (*p)->pos.y);
			
			hp->hexToHands = (*hands)->pos - hp->pos;

			float offsetMap = ofMap(distSqrd, 22500, 90000, 0, 1);
			ofClamp(offsetMap, 0,1);
			ofVec2f offsetAccel = hp->hexToHands*offsetMap;	

			if (distSqrd < 250000){				
				closestHand.push_back((*hands));	

				if(distSqrd > 90000){
					hp->accel += offsetAccel;
				}
				else if(distSqrd < 90000){
					hp->accel -= offsetAccel*0.002;
				}
			}
		}

		if(closestHand.size() != 0){
			for(vector<Particle*>::iterator connectedhands = closestHand.begin(); connectedhands != closestHand.end(); ++connectedhands){

				ofLine( (*connectedhands)->pos.x, (*connectedhands)->pos.y, (*p)->pos.x, (*p)->pos.y);
				
				hp->hexAlpha += 5;
				if(hp->hexAlpha > 200){
					hp->hexAlpha = 200;
				}
				
				hp->hexSize += hp->hexGrowthRate;
				if (hp->hexSize  > 1){  
					hp->hexSize = 1; 
				}
			}
			closestHand.clear();
		}
		else
		{

			hp->hexAlpha -= 5;
			if(hp->hexAlpha < 0){
				hp->hexAlpha = 0;
			}	

			hp->hexSize -= hp->hexGrowthRate;
				if (hp->hexSize  < 0.3){  
					hp->hexSize = 0.3; 
				}
		}
		hp->hexColor.a = hp->hexAlpha;	
	}

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

			if((*p)->pos.x < l->x+m_lightImg.width && (*p)->pos.x+m_hexImgBorder.width > l->x){
				l->isOn = true;
				m_lightsOnPixels.setColor(count+1, 1, ofColor(255));
			}
			count++;
		}
	}
	m_lightsOnTexture.loadData(m_lightsOnPixels, GL_LUMINANCE);
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