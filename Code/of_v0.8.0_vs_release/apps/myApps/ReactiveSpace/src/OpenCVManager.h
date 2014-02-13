#pragma once
#include "ofMain.h"
#include "ParticleMain.h"
#include "ofxOpenCv.h"
#include "IScene.h"

#define SIMULATE_CROWD 1

class OpenCVManager
{
	bool learnBG;
	int diffThresh;
	int lastFrame;

	vector<Particle*>* pPeople;
	IScene** ppCurrentScene;

#ifdef SIMULATE_CROWD
	int crowdLastGenerated;
#endif

	ofVideoGrabber m_vidGrabber;
	
	//image frames
	ofxCvColorImage m_prevImg;
	ofxCvColorImage m_curImg;

	//for tacking
	cv::SurfFeatureDetector m_detector;
	vector<cv::KeyPoint> m_keyPoints;
	//tracking vars
	/*CvPoint2D32f* m_corners;
	int m_numCorners;
	double m_cornerQualityLevel;
	double m_minCornerDistance;*/

public:
	OpenCVManager(vector<Particle*>* people, IScene** currentScene);

	void update(int deltaTime);
	void debugDraw();

	void learnBackground();

	~OpenCVManager();
};

