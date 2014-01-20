#include "ReactiveSpaceApp.h"

//--------------------------------------------------------------
void ReactiveSpaceApp::setup()
{
	//init time stuff
	stepTimeDelta = 0;
	stepTimeLast = ofGetElapsedTimeMillis();
	crowdLastGenerated = 0;

	pPeople = new vector<Particle>();

	m_scenes.push_back(new FirstScene(pPeople));

	pCurrentScene = m_scenes[0];
}

//--------------------------------------------------------------
void ReactiveSpaceApp::update(){

	//get time since last step
	float stepTime = ofGetElapsedTimeMillis();
	stepTimeDelta = stepTime - stepTimeLast;
	
	//simulate crowd if necessary
	if (SIMULATE_CROWD)
	{
		//generate a person each second
		if (stepTime - crowdLastGenerated > 1000)
		{
			Particle p = Particle();
			int winH = ofGetWindowHeight();
			p.pos.y = ofRandom(winH * 0.25, winH * 0.75);
			p.pos.z = ofRandom(500);
			p.speed.x = ofRandom(0.1f, 1.f);
			pPeople->push_back(p);
			crowdLastGenerated = stepTime;
		}

		//move people across screen or remove them
		for (vector<Particle>::iterator p = pPeople->begin(); p != pPeople->end(); )
		{
			p->pos.x += p->speed.x * 0.1f * stepTimeDelta;

			if (p->pos.x > ofGetWindowWidth())
			{
				p = pPeople->erase(p);
			}
			else
			{
				++p;
			}
		}
	}

	//set last step time to current time
	stepTimeLast = stepTime;
}

//--------------------------------------------------------------
void ReactiveSpaceApp::draw(){
	pCurrentScene->Render();
}

//--------------------------------------------------------------
void ReactiveSpaceApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ReactiveSpaceApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ReactiveSpaceApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ReactiveSpaceApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ReactiveSpaceApp::mousePressed(int x, int y, int button)
{
	Particle p;
	p.pos = ofPoint(x, y);
	pPeople->push_back(p);
}

//--------------------------------------------------------------
void ReactiveSpaceApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ReactiveSpaceApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ReactiveSpaceApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ReactiveSpaceApp::dragEvent(ofDragInfo dragInfo){

}
