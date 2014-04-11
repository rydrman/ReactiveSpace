#include "LightScene.h"

const ofVec2f s_fogTexCoords[] = {
	ofVec2f(0.0f, 0.0f),
	ofVec2f(1.0f, 0.0f),
	ofVec2f(1.0f, 1.0f),
	ofVec2f(0.0f, 1.0f)
};

LightScene::LightScene(vector<Particle*>* people, vector<Particle*>* hands, AudioManager* audioManager, ImageManager* imageManager)
: IScene(people, hands, audioManager, imageManager)
{
	const ofVec2f fogVerts[] = {
		ofVec2f(0.f, 0.f),
		ofVec2f(ofGetWidth(), 0.f),
		ofVec2f(ofGetWidth(), ofGetHeight()),
		ofVec2f(0.0f, ofGetHeight())
	};

	pBackgroundSound = pAudioManager->load("LightScene/Scene2_Background2.mp3");
	pBackgroundSound->setLoop(true);

	//for hands
	m_connectedToHands = false;
	m_connectedParticles = vector<HexagonParticle*>();

	//load background image
	m_backgroundImg = pImageManager->load("LightScene/lightsBackground.png");

	//load hex images
	m_hexImgBorder = pImageManager->load("LightScene/Hexagon.png");
	m_hexImgInner = pImageManager->load("LightScene/hexagonFill.png");
	m_hexLineImg = pImageManager->load("LightScene/hexLine1_FINAL.png", false);

	//load light images
	m_lightImg = pImageManager->load("LightScene/light.png");
	m_lightTube = pImageManager->load("LightScene/lightTube.png");

	//load hand image
	m_handsImage = pImageManager->load("LightScene/handsImage.png");

	//for fog
	m_fogShader.load("LightScene/fogShader");
	m_fogImg.loadImage("LightScene/fog.png");
	m_lightAlpha.loadImage("LightScene/lightAlpha.png");
	m_fogAlphaMask.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);

	m_fogVbo.disableVAOs();
	m_fogVbo.setVertexData(fogVerts, 4, GL_STATIC_DRAW);
	m_fogVbo.setTexCoordData(s_fogTexCoords, 4, GL_STATIC_DRAW);
	
	//for lights
	m_lights = vector<Light>();
	for(int i = 0; i < ofGetWidth(); i += m_lightImg->width * 0.7){
		Light l = Light();
		l.x = i;
		m_lights.push_back(l);	
	}
}

void LightScene::Render()
{
	ofClear(0);

	//draw lights for background
	ofSetColor(255);
	ofSetRectMode(OF_RECTMODE_CORNER);
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	for (vector<Light>::iterator l = m_lights.begin(); l != m_lights.end(); ++l){
		if (l->isOn() == true){
			m_lightImg->draw(l->x - m_lightImg->width * 0.5, 0, 0);
		}
	}
	ofDisableBlendMode();

	//draw background image
	ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
	m_backgroundImg->draw(0, 0);
	ofDisableBlendMode();

	ofEnableAlphaBlending();

	//draw light tubes
	for (vector<Light>::iterator l = m_lights.begin(); l != m_lights.end(); ++l){
		if (l->isOn() == true){
			m_lightTube->draw(l->x - m_lightImg->width * 0.5, 0, 0);
		}
	}


	HexagonParticle* hp;
	//draw a hexagon for each person in people
	for (vector<Particle*>::iterator p = pPeople->begin(); p != pPeople->end(); ++p)
	{
		hp = (HexagonParticle*)(*p);

		ofPushMatrix();
		ofTranslate(hp->pos); //translate to people pos
		ofSetRectMode(OF_RECTMODE_CENTER);
		ofRotate(hp->hexRotation); //give random rotation
		ofScale(hp->hexSize, hp->hexSize); //set scale, this changes if connected
		ofSetColor(hp->hexColor); //set color, this changes if connected
		m_hexImgInner->draw(0, 0);

		ofSetColor(255);
		m_hexImgBorder->draw(0, 0);
		ofPopMatrix();

		ofSetRectMode(OF_RECTMODE_CORNER);
		//if connected and m_closest hand has a size
		if (hp->isConnected){
			if (m_closestHand.size() != 0){
				for (vector<Particle*>::iterator connectedhands = hp->connectedHands.begin(); connectedhands != hp->connectedHands.end(); ++connectedhands){

					//draw lines from hands to connected particles
					ofPushMatrix();
					float angle = (atan2((*connectedhands)->pos.y - hp->pos.y, (*connectedhands)->pos.x - hp->pos.x) * 180 / PI)+90; //offset by 90 since image is pointing up
					float dist = ofDist((*connectedhands)->pos.x*0.5, (*connectedhands)->pos.y*0.5, hp->pos.x*0.5, hp->pos.y*0.5);
					ofTranslate((*connectedhands)->pos.x, (*connectedhands)->pos.y);
					ofRotate(angle);
					ofScale(1, dist);
					m_hexLineImg->draw(0,0);
					ofPopMatrix();
				}
			}
		}
	}

	//FOG 
	ofSetColor(255);
	ofSetRectMode(OF_RECTMODE_CORNER);

	m_fogAlphaMask.begin();
	//ofEnableAlphaBlending();
	ofClear(0, 0);
	int dif = m_lightAlpha.width * 0.5;
	for (vector<Light>::iterator l = m_lights.begin(); l != m_lights.end(); ++l){
		if (l->isOn() == true){
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

	//draw hand images
	ofSetRectMode(OF_RECTMODE_CENTER);
	for (vector<Particle*>::iterator hands = pHandPositions->begin(); hands != pHandPositions->end(); ++hands){
		ofPushMatrix();
		ofTranslate((*hands)->pos);
		m_handsImage->draw(0, 0);
		ofPopMatrix();
	}
	ofSetRectMode(OF_RECTMODE_CORNER);
	m_closestHand.clear(); // set here so each frame the closest hand resets
}

void LightScene::Update(float timeScale)
{

	HexagonParticle* hp;
	for(vector<Light>::iterator l = m_lights.begin(); l != m_lights.end(); ++l)
	{
		bool turnedOn = false;
		for (vector<Particle*>::iterator p = pPeople->begin(); p != pPeople->end(); ++p)
		{
			hp = (HexagonParticle*)(*p);
			if (hp->pos.x < l->x + m_lightImg->width*0.5 && hp->pos.x + (m_hexImgBorder->width*hp->hexSize)*0.5 > l->x)
			{
				l->turnOn();
				turnedOn = true;
				break;
			}
		}
		if (!turnedOn)
		{
			l->turnOff();
		}
		l->update(timeScale);
	}

	m_connectedParticles.clear();
	for (vector<Particle*>::iterator p = pPeople->begin(); p != pPeople->end(); ++p)
	{
		hp = (HexagonParticle*)(*p);
		hp->isConnected = false;
		hp->connectedHands.clear();

		float distSqrd = std::numeric_limits<float>::max();
		for (vector<Particle*>::iterator hands = pHandPositions->begin(); hands != pHandPositions->end(); ++hands){

			distSqrd = ofDistSquared((*hands)->pos.x, (*hands)->pos.y, (*p)->pos.x, (*p)->pos.y);

			hp->hexToHands = (*hands)->pos - hp->pos;

			float offsetMap = ofMap(distSqrd, 22500.f, 90000.f, 0.f, 1.f);
			ofClamp(offsetMap, 0.f, 1.f);

			ofVec2f offsetAccel = hp->hexToHands*offsetMap;

			//if close to hands connect them
			if (distSqrd < 150000){
				hp->isConnected = true;
				hp->connectedHands.push_back((*hands));
				m_closestHand.push_back((*hands));

				//add to connected particles array and apply separation
				m_connectedParticles.push_back(hp);
							
				//apply tug and pull effect to particles when connected to hands
				if (distSqrd > 90000){
					hp->applyForce(offsetAccel * distSqrd-90000);
				}
				else if (distSqrd < 90000){
					hp->applyForce(offsetAccel * distSqrd-90000);
				}
			}
			hp->separation(&m_connectedParticles);
		}

		if (hp->isConnected){					
				hp->hexAlpha += 5;
				if (hp->hexAlpha > 200){
					hp->hexAlpha = 200;
				}

				hp->hexSize += hp->hexGrowthRate;
				if (hp->hexSize > 1){
					hp->hexSize = 1;
				}
		}
		else
		{
			//decrements the alpha and the size
			hp->hexAlpha -= 5;
			if (hp->hexAlpha < 0){
				hp->hexAlpha = 0;
			}

			hp->hexSize -= hp->hexGrowthRate;
			if (hp->hexSize < 0.3){
				hp->hexSize = 0.3;
			}
		}
		hp->hexColor.a = hp->hexAlpha;
	}
}

void LightScene::convertPeopleVector()
{
	vector<Particle*> newPeople = vector<Particle*>();

	for (vector<Particle*>::iterator pOld = pPeople->begin(); pOld != pPeople->end(); ++pOld)
	{
		HexagonParticle* p = new HexagonParticle((*pOld)->pos);
		p->maxForce = 1.f;
		newPeople.push_back(p);
	}
	*pPeople = newPeople;
}
Particle* LightScene::addParticleOfProperType(ofVec3f _pos)
{
	HexagonParticle* p = new HexagonParticle(_pos);
	p->maxForce = 1.f;
	pPeople->push_back(p);
	return p;
}

void LightScene::onLoad()
{
	pBackgroundSound->play();
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

void LightScene::onUnload()
{
	pBackgroundSound->stop();
}

LightScene::~LightScene()
{

}