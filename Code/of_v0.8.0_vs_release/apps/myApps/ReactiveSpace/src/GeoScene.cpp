

#include "GeoScene.h"


static const int radius = 100;
static ofColor col = ofColor(255, 255, 255);
 int startTime = 0;
GeoScene::GeoScene(vector<Particle*>* people, vector<Particle*>* hands)
: IScene(people, hands)
{
	screenW = ofGetWidth();
	screenH = ofGetHeight();	

	//geo1.loadImage("GeoScene/Geometric1.png");
	//geo2.loadImage("GeoScene/Geometric2.png");
	//geo3.loadImage("GeoScene/Geometric3.png");
	geoBack.loadImage("GeoScene/Geometric_Background.jpg");
	geoHands.loadImage("GeoScene/Hand_White.png");

	/*GeoParticle* gp;
	for (vector<Particle*>::iterator p = pPeople->begin(); p != pPeople->end(); ++p)
	{
	gp = (GeoParticle*)(*p);
		int whichGeo = ofRandom(1,3);
		if(whichGeo == 1){
			gp->m_shapes = geo1;
		}
		if(whichGeo == 2){
			gp->m_shapes = geo2;
		}
		if(whichGeo == 3){
			gp->m_shapes = geo3;
		}
		
	}*/

	m_gradients.loadImage("GeoScene/gradients.png");

}

void GeoScene::Render()
{
	ofBackground(0, 255);
	
	ofPushMatrix();
		//ofTranslate(0,0);
		//ofScale(screenW, screenH);
		geoBack.draw(0,0,0);
	ofPopMatrix();

	for (vector<Particle*>::iterator h = pHandPositions->begin(); h != pHandPositions->end(); ++h)
	{
		ofPushMatrix();
			//ofSetColor(0,200,200,70);
			//ofScale(radius,radius);
			geoHands.draw((*h)->pos.x-185,(*h)->pos.y-185,0);
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
			gp->draw();
		//ofPopMatrix();
	}

	m_gradients.unbind();
}

void GeoScene::Update(int deltaTime)
{
	//update hands
	GeoParticle* gp;
	for (vector<Particle*>::iterator h = pHandPositions->begin(); h != pHandPositions->end(); ++h)
	{

		for (vector<Particle*>::iterator p = pPeople->begin(); p != pPeople->end(); ++p)
		{
			gp = (GeoParticle*)(*p);
			
			if ((*h)->pos.x > ((*p)->pos.x - radius) && (*h)->pos.x <((*p)->pos.x + radius))
			{
				if ((*h)->pos.y >((*p)->pos.y - radius) && (*h)->pos.y < ((*p)->pos.y + radius))
				{
					//(*p)->vel.x = 0;
				    //(*p)->vel.y = 0;
					gp->countDown(deltaTime);
					geoHands.loadImage("GeoScene/Hand_Green.png");
					
				}
			}
			else{
				(*p)->vel.x = ofRandom(0.3,1);
				col = ofColor(255, 255, 255);


			}
		}

	}

	//update geo particles
	for (vector<Particle*>::iterator p = pPeople->begin(); p != pPeople->end(); ++p)
	{
		gp = (GeoParticle*)(*p);
		gp->update();
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

GeoScene::~GeoScene()
{
}
