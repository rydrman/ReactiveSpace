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

	m_connectedToHands = false;

	//load all images 
	m_hexImgBorder.loadImage("LightScene/Hexagon.png");
	m_hexImgInner.loadImage("LightScene/hexagonFill.png");
	m_lightImg.loadImage("LightScene/light.png"); 
	m_lightTube.loadImage("LightScene/lightTube.png");
	m_backgroundImg.loadImage("LightScene/lightsBackground.png");
	m_handsImage.loadImage("LightScene/handsImage.png");
	m_hexLineImg.loadImage("LightScene/hexLine.png");

	//for fog
	m_fogShader.load("LightScene/fogShader");
	m_fogImg.loadImage("LightScene/fog.png");
	m_lightAlpha.loadImage("LightScene/lightAlpha.png");
	m_fogAlphaMask.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);

	m_fogVbo.disableVAOs();
	m_fogVbo.setVertexData(fogVerts, 4, GL_STATIC_DRAW);
	m_fogVbo.setTexCoordData(s_fogTexCoords, 4, GL_STATIC_DRAW);

	m_lights = vector<Light>();

	for(int i = 0; i < ofGetWidth(); i += m_lightImg.width * 0.7){
		Light l = Light();
		l.isOn = false;
		l.x = i;
		m_lights.push_back(l);	
	}

	m_connectedParticles = vector<HexagonParticle*>();
}

void LightScene::Render()
{
	ofClear(0);

	//draw lights for background
	ofSetColor(255);
	ofSetRectMode(OF_RECTMODE_CORNER);
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	for (vector<Light>::iterator l = m_lights.begin(); l != m_lights.end(); ++l){
		if (l->isOn == true){
			m_lightImg.draw(l->x - m_lightImg.width * 0.5, 0, 0);
		}
	}
	ofDisableBlendMode();

	ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
	m_backgroundImg.draw(0, 0);
	ofDisableBlendMode();

	ofEnableAlphaBlending();

	//draw light tubes
	for (vector<Light>::iterator l = m_lights.begin(); l != m_lights.end(); ++l){
		if (l->isOn == true){
			m_lightTube.draw(l->x - m_lightImg.width * 0.5, 0, 0);
		}
	}

	m_connectedParticles.clear();
	HexagonParticle* hp;
	//draw a hexagon for each person in people
	for (vector<Particle*>::iterator p = pPeople->begin(); p != pPeople->end(); ++p)
	{
		hp = (HexagonParticle*)(*p);

		//update
		hp->hexColor.a = hp->hexAlpha;

		//draw hexagon
		ofPushMatrix();
		ofTranslate(hp->pos);
		ofSetRectMode(OF_RECTMODE_CENTER);
		ofRotate(hp->hexRotation);

		//fill
		ofScale(hp->hexSize, hp->hexSize);
		ofSetColor(hp->hexColor);
		m_hexImgInner.draw(0, 0);

		//outline
		ofSetColor(255);
		m_hexImgBorder.draw(0, 0);
		ofPopMatrix();
		ofSetRectMode(OF_RECTMODE_CORNER);
	}
		//FOG 
		ofSetColor(255);
		ofSetRectMode(OF_RECTMODE_CORNER);

		m_fogAlphaMask.begin();
		//ofEnableAlphaBlending();
		ofClear(0, 0);
		int dif = m_lightAlpha.width * 0.5;
		for (vector<Light>::iterator l = m_lights.begin(); l != m_lights.end(); ++l){
			if (l->isOn == true){
				m_lightAlpha.draw(l->x - dif, 0, 0);
			}
		}
		m_fogAlphaMask.end();

		//draw fog with shader
		m_fogShader.begin();

		m_fogImg.getTextureReference().bind();
		m_fogShader.setUniformTexture("imageMask", m_fogAlphaMask.getTextureReference(), 1);
		m_fogVbo.bind();

		glDrawArrays(GL_QUADS, 0, 4);

		m_fogImg.getTextureReference().unbind();
		m_fogVbo.unbind();

		m_fogShader.end();

		//draw hand circles
		ofSetRectMode(OF_RECTMODE_CENTER);
		for (vector<Particle*>::iterator hands = pHandPositions->begin(); hands != pHandPositions->end(); ++hands){
			ofPushMatrix();
			ofTranslate((*hands)->pos);
			m_handsImage.draw(0, 0);
			ofPopMatrix();
		}
		ofSetRectMode(OF_RECTMODE_CORNER);
}

void LightScene::Update(int deltaTime)
{
	
	for(vector<Light>::iterator l = m_lights.begin(); l != m_lights.end(); ++l){
		l->isOn = false;
	}

	int count = 0;
	HexagonParticle* hp;
	m_connectedParticles.clear();
	//draw a hexagon for each person in people
	for (vector<Particle*>::iterator p = pPeople->begin(); p != pPeople->end(); ++p)
	{
		hp = (HexagonParticle*)(*p);

		if (hp->isConnected)
		{
			hp->separation(&m_connectedParticles);
		}

		for (vector<Light>::iterator l = m_lights.begin(); l != m_lights.end(); ++l){

			if (hp->pos.x < l->x + m_lightImg.width*0.5 && hp->pos.x + (m_hexImgBorder.width*hp->hexSize)*0.5 > l->x){
				l->isOn = true;
			}
			count++;
		}

		float distSqrd = std::numeric_limits<float>::max();

		for (vector<Particle*>::iterator hands = pHandPositions->begin(); hands != pHandPositions->end(); ++hands)
		{

			//distance from hands to particle 
			distSqrd = ofDistSquared((*hands)->pos.x, (*hands)->pos.y, (*p)->pos.x, (*p)->pos.y);

			//vector from particle to hands
			hp->hexToHands = (*hands)->pos - hp->pos;

			float offsetMap = ofMap(distSqrd, 22500, 90000, 0, 1);
			ofClamp(offsetMap, 0, 1);
			ofVec2f offsetAccel = hp->hexToHands*offsetMap;

			if (distSqrd < 250000)
			{

				m_closestHand.push_back((*hands));

				hp->isConnected = true;
				m_connectedParticles.push_back(hp);


				if (distSqrd > 90000){
					hp->accel += offsetAccel;
				}
				else if (distSqrd < 90000){
					hp->accel -= offsetAccel*0.0025;
				}
				
			}
			else
			{
				hp->isConnected = false;
			}
		}

		if (m_closestHand.size() != 0){
			for (vector<Particle*>::iterator connectedhands = m_closestHand.begin(); connectedhands != m_closestHand.end(); ++connectedhands){

		//		ofPushMatrix();
		//			float angle = atan2((*connectedhands)->pos.y - hp->pos.y, (*connectedhands)->pos.x - hp->pos.x) * 180 / PI;;
		//			float dist = ofDist((*connectedhands)->pos.x, (*connectedhands)->pos.y, hp->pos.x, hp->pos.y);
			//		ofRotate(angle);
			//		m_hexLineImg.draw((*connectedhands)->pos.x, (*connectedhands)->pos.y, m_hexLineImg.width, dist);
			//	ofPopMatrix();

				//increments the alpha and the size
				hp->hexAlpha += 5;
				if (hp->hexAlpha > 200){
					hp->hexAlpha = 200;
				}

				hp->hexSize += hp->hexGrowthRate;
				if (hp->hexSize  > 1){
					hp->hexSize = 1;
				}
			}
			m_closestHand.clear();
		}
		else
		{
			//decrements the alpha and the size
			hp->hexAlpha -= 5;
			if (hp->hexAlpha < 0){
				hp->hexAlpha = 0;
			}

			hp->hexSize -= hp->hexGrowthRate;
			if (hp->hexSize  < 0.3){
				hp->hexSize = 0.3;
			}
			m_connectedToHands = false;
		}
	}
}

void LightScene::convertPeopleVector()
{
	vector<Particle*> newPeople = vector<Particle*>();

	for (vector<Particle*>::iterator pOld = pPeople->begin(); pOld != pPeople->end(); ++pOld)
	{
		HexagonParticle* p = new HexagonParticle((*pOld)->pos);
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

void LightScene::onLoad()
{
	/*m_fogVbo.

	const ofVec2f fogVerts[] = {
		ofVec2f(0.f, 0.f),
		ofVec2f(ofGetWidth(), 0.f),
		ofVec2f(ofGetWidth(), ofGetHeight()),
		ofVec2f(0.0f, ofGetHeight())
	};

	m_fogVbo.setVertexData(fogVerts, 4, GL_STATIC_DRAW);
	m_fogVbo.setTexCoordData(s_fogTexCoords, 4, GL_STATIC_DRAW);*/
}

LightScene::~LightScene()
{

}