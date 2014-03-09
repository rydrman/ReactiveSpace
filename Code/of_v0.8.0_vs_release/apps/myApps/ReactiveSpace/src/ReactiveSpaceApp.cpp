#include "ReactiveSpaceApp.h"

void ReactiveSpaceApp::setup()
{
	//setup vars
	m_currentSceneNum = 3;

	//graphics
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground(0, 0, 0);
	ofDisableArbTex();

	//init time stuff
	stepTimeDelta = 0;
	stepTimeLast = ofGetElapsedTimeMillis();

	//open CV stuff
	pCurrentScene = nullptr;
	pPeople = vector<Particle*>();
	openCVManager = new OpenCVManager(&pPeople, &pCurrentScene);

	//kinect
	pHandPositions = vector<Particle*>();
	kinectManager = new KinectManager(&pHandPositions, &pCurrentScene);
	kinectManager->initialize();

	//fill scene vector
	m_scenes.push_back(new GridScene(&pPeople, &pHandPositions));
	m_scenes.push_back(new LightScene(&pPeople, &pHandPositions));
	m_scenes.push_back(new RainScene(&pPeople, &pHandPositions));
	m_scenes.push_back(new GeoScene(&pPeople, &pHandPositions));
	pCurrentScene = m_scenes[m_currentSceneNum];
	pCurrentScene->convertPeopleVector();

#ifdef DEBUG_DRAW
	kinectUpdateMS = 0;
	openCVUpdateMS = 0;
	sceneUpdateMS = 0;
	sceneDrawMS = 0;
#endif
}

//--------------------------------------------------------------
void ReactiveSpaceApp::update()
{
	//get time since last step
	float stepTime = ofGetElapsedTimeMillis();
	stepTimeDelta = stepTime - stepTimeLast;

#ifdef DEBUG_DRAW
	long newMS = kinectUpdateMS;
	newMS = ofGetSystemTimeMicros();
#endif

	//update kinect
	HRESULT hr = kinectManager->update(stepTimeDelta);
	if (FAILED(hr) && !kinectManager->isFailed())
	{
		cout << "\nkinect failed to update frame";
		//do something so user knows
	}

#ifdef DEBUG_DRAW
	newMS = ofGetSystemTimeMicros() - newMS;
	kinectUpdateMS = (kinectUpdateMS * 3 + newMS) * 0.25;
	newMS = ofGetSystemTimeMicros();
#endif

	//update open cv
	openCVManager->update(stepTimeDelta);

#ifdef DEBUG_DRAW
	newMS = ofGetSystemTimeMicros() - newMS;
	openCVUpdateMS = (openCVUpdateMS * 3 + newMS) * 0.25;
#endif

	//set last step time to current time
	stepTimeLast = stepTime;

	//step scene
#ifdef DEBUG_DRAW
	newMS = ofGetSystemTimeMicros();
#endif

	pCurrentScene->Update(stepTimeDelta);

#ifdef DEBUG_DRAW
	newMS = ofGetSystemTimeMicros() - newMS;
	sceneUpdateMS = (sceneUpdateMS * 3 + newMS) * 0.25;
#endif
}

//--------------------------------------------------------------
void ReactiveSpaceApp::draw()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT || GL_DEPTH_BUFFER_BIT);
	ofSetColor(255);

#ifdef DEBUG_DRAW
	long newMS = ofGetSystemTimeMicros();
#endif

	pCurrentScene->Render();

#ifdef DEBUG_DRAW
	newMS = ofGetSystemTimeMicros() - newMS;
	sceneDrawMS = (sceneDrawMS * 3 + newMS) * 0.25;
#endif


#ifdef DEBUG_DRAW
	ofPushMatrix();
	ofSetColor(200, 255, 0, 255);
	ofFill();

	ofDrawBitmapString("Framerate: " + ofToString(ofGetFrameRate(), 2), 10.f, 10.f);
	ofDrawBitmapString("Kinect Update Time: " + ofToString(kinectUpdateMS * 0.001f, 1), 10.f, 25.f);
	ofDrawBitmapString("OpenCV Update Time: " + ofToString(openCVUpdateMS * 0.001f, 1), 10.f, 40.f);
	ofDrawBitmapString("Scene Update Time: " + ofToString(sceneUpdateMS * 0.001f, 1), 10.f, 55.f);
	ofDrawBitmapString("Scene Draw Time: " + ofToString(sceneDrawMS * 0.001f, 1), 10.f, 70.f);
	ofPopMatrix();

	openCVManager->debugDraw();
#endif
}

//--------------------------------------------------------------
void ReactiveSpaceApp::keyPressed(int key)
{
	switch (key){
	case 'b':
		openCVManager->learnBackground();
		break;
	case '0':
		m_currentSceneNum = 0;
		pCurrentScene = m_scenes[m_currentSceneNum];
		pCurrentScene->convertPeopleVector();
		pCurrentScene->convertHandVector();
		break;
	case '1':
		m_currentSceneNum = 1;
		pCurrentScene = m_scenes[m_currentSceneNum];
		pCurrentScene->convertPeopleVector();
		pCurrentScene->convertHandVector();
		break;
	case '2':
		m_currentSceneNum = 2;
		pCurrentScene = m_scenes[m_currentSceneNum];
		pCurrentScene->convertPeopleVector();
		pCurrentScene->convertHandVector();
		break;
	case '3':
		m_currentSceneNum = 3;
		pCurrentScene = m_scenes[m_currentSceneNum];
		pCurrentScene->convertPeopleVector();
		pCurrentScene->convertHandVector();
		break;
	}
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
	if (kinectManager->isFailed())
	{
		ofVec3f pos = ofVec3f(x, y);
		Particle* p;
		if (pHandPositions.size() == 0)
		{
			p = pCurrentScene->addHandOfProperType(pos);
			p->timer = 0;
		}
		else
		{
			p = pHandPositions.at(0);
			p->vel = pos - p->pos;
			p->timer = 0;
			p->update();
		}
	}
}

//--------------------------------------------------------------
void ReactiveSpaceApp::mousePressed(int x, int y, int button)
{

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
