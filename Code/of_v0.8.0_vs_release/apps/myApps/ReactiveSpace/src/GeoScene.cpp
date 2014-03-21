

#include "GeoScene.h"


 int radius = 100;
 bool onTarget = false;
static ofColor col = ofColor(255, 255, 255);
 int startTime = 0;
GeoScene::GeoScene(vector<Particle*>* people, vector<Particle*>* hands)
: IScene(people, hands)
{

	screenW = ofGetWidth();
	screenH = ofGetHeight();	

	geoBack.loadImage("GeoScene/Geometric_Background.jpg");
	geoHands.loadImage("GeoScene/Hand_White.png");

	m_gradients.loadImage("GeoScene/gradients.png");


	explosionSprite.loadImage("GeoScene/Particles2.png");

	geoBackSound.loadSound("GeoScene/Scene3Back.mp3");
	geoBackSound.setLoop(true); 
	geoBackSound.play();
	

}

void GeoScene::Render()
{
	ofBackground(0, 255);
	ofSetColor(255);
	ofPushMatrix();
		geoBack.draw(0,0,0);
	ofPopMatrix();

	GeoHands* gh;
	for (vector<Particle*>::iterator h = pHandPositions->begin(); h != pHandPositions->end(); ++h)
	{
		gh = (GeoHands*)(*h);
		ofPushMatrix();
			//ofSetColor(0,200,200,70);
			//ofScale(radius,radius);
			gh->drawHands(geoHands);
			
		ofPopMatrix();
	}

	GeoParticle* gp;

	m_gradients.bind();

	for (vector<Particle*>::iterator p = pPeople->begin(); p != pPeople->end(); ++p)
	{
		gp = (GeoParticle*)(*p);
		ofVec4f  position = (*p)->pos;
		
		//ofPushMatrix();
			
		    //ofTranslate((*p)->pos);
			//ofSetColor(col);
			gp->draw(m_gradients, explosionSprite);
		//ofPopMatrix();

	}

	m_gradients.unbind();



}

void GeoScene::Update(int timeScale)
{
	//update hands
	GeoParticle* gp;
	GeoHands* gh;
	for (vector<Particle*>::iterator h = pHandPositions->begin(); h != pHandPositions->end(); ++h)
	{
		gh= (GeoHands*)(*h);
		onTarget = false;

		for (vector<Particle*>::iterator p = pPeople->begin(); p != pPeople->end(); ++p)
		{
			gp = (GeoParticle*)(*p);
			if(gp->m_isExploding) continue;
			

			if ((*h)->pos.x + radius > ((*p)->pos.x ) && (*h)->pos.x - radius<((*p)->pos.x ))
			{
				
				if ((*h)->pos.y -50 + radius >((*p)->pos.y  ) && (*h)->pos.y - radius < ((*p)->pos.y))
				{
					

					//gp->vel.x = gp->vel.x /2 ;
					gp->countDown(timeScale);
					onTarget = true;
					gh->handCountDown(timeScale, onTarget);	

					if( gp->m_isDead){
						pPeople->erase(p, p);
					}

					goto stopSearching;
				}
			}
			


		}

	}

	stopSearching:

	//update geo particles
	for (vector<Particle*>::iterator p = pPeople->begin(); p != pPeople->end(); ++p)
	{
		gp = (GeoParticle*)(*p);
		gp->update(timeScale);
		
	}	

}


void GeoScene::convertPeopleVector()
{
	vector<Particle*> newPeople = vector<Particle*>();

	for (vector<Particle*>::iterator pOld = pPeople->begin(); pOld != pPeople->end(); ++pOld)
	{
		GeoParticle* p = new GeoParticle(**pOld._Ptr);
		newPeople.push_back(p);
	}
	*pPeople = newPeople;
}
Particle* GeoScene::addParticleOfProperType(ofVec3f _pos)
{
	GeoParticle* p = new GeoParticle(_pos);
	pPeople->push_back(p);
	return p;
}

void GeoScene::convertHandVector()
{
	vector<Particle*> newHands = vector<Particle*>();

	for (vector<Particle*>::iterator hOld = pHandPositions->begin(); hOld != pHandPositions->end(); ++hOld)
	{
		Particle* h = new GeoHands(**hOld._Ptr);
		newHands.push_back(h);
	}
	*pPeople = newHands;
}
Particle* GeoScene::addHandOfProperType(ofVec3f _pos)
{
	Particle* h = new GeoHands(_pos);
	pHandPositions->push_back(h);
	return h;
}











GeoScene::~GeoScene()
{
}

