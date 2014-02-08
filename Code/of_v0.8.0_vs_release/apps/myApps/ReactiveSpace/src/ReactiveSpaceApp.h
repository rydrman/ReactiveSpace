#pragma once

#define DEBUG_DRAW 1

#include "ofMain.h"
#include <stdio.h>
#include <vector>
#include "IScene.h"
#include "GridScene.h"
#include "RainScene.h"
#include "GeoScene.h"
#include "LightScene.h"
#include "ParticleMain.h"
#include "KinectManager.h"
#include "OpenCVManager.h"

using std::vector;

class ReactiveSpaceApp : public ofBaseApp{

	int m_currentSceneNum;
	vector<IScene*> m_scenes;
	IScene* pCurrentScene;

	float stepTimeDelta;
	float stepTimeLast;

	//for kinect
	KinectManager* kinectManager;
	vector<Particle*> pHandPositions;

	//for openCV
	OpenCVManager* openCVManager;
	vector<Particle*> pPeople;
	
	//for crowd simulation
	int crowdLastGenerated;

	//for debugging
#ifdef DEBUG_DRAW
	long kinectUpdateMS;
	long openCVUpdateMS;
	long sceneUpdateMS;
	long sceneDrawMS;
#endif

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
};
