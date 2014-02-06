#include "ReactiveSpaceApp.h"
#include <stdio.h>

//--------------------------------------------------------------
void ReactiveSpaceApp::setup()
{
	//init time stuff
	stepTimeDelta = 0;
	stepTimeLast = ofGetElapsedTimeMillis();
	crowdLastGenerated = 0;

	//open CV stuff
	pPeople = new vector<Particle>();

	//kinect
	pHandPositions = new vector<Vector4>();
	kinectManager = new KinectManager();
	kinectManager->initialize();

	//fill scene vector
	m_scenes.push_back(new GridScene(pPeople, pHandPositions));
	m_scenes.push_back(new LightScene(pPeople, pHandPositions));
	m_scenes.push_back(new RainScene(pPeople, pHandPositions));
	m_scenes.push_back(new GeoScene(pPeople, pHandPositions));
	pCurrentScene = m_scenes[1];

	//graphics
	//ofDisableArbTex();
	ofSetFrameRate(60);
	ofSetVerticalSync(false);
	ofBackground(0, 0, 0);
}

//--------------------------------------------------------------
void ReactiveSpaceApp::update()
{
	//get time since last step
	float stepTime = ofGetElapsedTimeMillis();
	stepTimeDelta = stepTime - stepTimeLast;

	//update kinect
	HRESULT hr = kinectManager->update( stepTimeDelta, pHandPositions );

	if (FAILED(hr))
	{
		//do something so user knows
	}

	//simulate crowd if necessary
	if (SIMULATE_CROWD)
	{
		//generate a person each second
		if (stepTime - crowdLastGenerated > 5000)
		{
			Particle p = Particle();
			int winH = ofGetWindowHeight();
			p.pos.y = ofRandom(winH * 0.25, winH * 0.75);
			p.vel = ofVec2f(ofRandom(0.3f, 1.f), 0.f);
			pPeople->push_back(p);
			crowdLastGenerated = stepTime;
		}

		//move people across screen or remove them
		for (vector<Particle>::iterator p = pPeople->begin(); p != pPeople->end(); )
		{
			particle_updatePosition((Particle*)p._Ptr);// stepTimeDelta;

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

	//step scene
	pCurrentScene->Update(stepTimeDelta);
}

//--------------------------------------------------------------
void ReactiveSpaceApp::draw()
{
	glClearColor( 0.0, 0.0, 0.0, 1.0 );
	glClear( GL_COLOR_BUFFER_BIT || GL_DEPTH_BUFFER_BIT );

	pCurrentScene->Render();

	glDisable( GL_CULL_FACE );
	

	//debug stuff
	ofPushMatrix();
		ofSetColor(200, 255, 0, 255);
		ofFill();
		ofDrawBitmapString("Framerate: " + ofToString(ofGetFrameRate(), 2), 10.f, 10.f);
	ofPopMatrix();
}

//--------------------------------------------------------------
void ReactiveSpaceApp::keyPressed(int key)
{

}

//--------------------------------------------------------------
void ReactiveSpaceApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void ReactiveSpaceApp::mouseMoved(int x, int y)
{

}

//--------------------------------------------------------------
void ReactiveSpaceApp::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ReactiveSpaceApp::mousePressed(int x, int y, int button)
{
	Particle p;
	p.pos = ofPoint(x, y);
	pPeople->push_back(p);
}

//--------------------------------------------------------------
void ReactiveSpaceApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ReactiveSpaceApp::windowResized(int w, int h)
{

}

//--------------------------------------------------------------
void ReactiveSpaceApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void ReactiveSpaceApp::dragEvent(ofDragInfo dragInfo)
{

}
