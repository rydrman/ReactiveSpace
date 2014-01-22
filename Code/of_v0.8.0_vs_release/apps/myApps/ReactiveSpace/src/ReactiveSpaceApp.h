#pragma once

#include "ofMain.h"
#include <vector>
#include "IScene.h"
#include "GridScene.h"
#include "Particle.cpp"

using std::vector;

class ReactiveSpaceApp : public ofBaseApp{

	vector<Particle>* pPeople;

	vector<IScene*> m_scenes;
	IScene* pCurrentScene;

	static const bool SIMULATE_CROWD = true;
	static const bool SIMULATE_GESTURE = true;

	float stepTimeDelta;
	float stepTimeLast;
	
	//for crowd simulation
	int crowdLastGenerated;

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
