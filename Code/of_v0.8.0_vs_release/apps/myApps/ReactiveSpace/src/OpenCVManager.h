#pragma once
#include "ofMain.h"
#include "ParticleMain.h"
#include "ofxOpenCv.h"
#include "IScene.h"


class OpenCVManager
{
	bool learnBG;
	int diffThresh;
	int lastFrame;

	vector<Particle*>* pPeople;
	IScene** ppCurrentScene;

	int crowdLastGenerated;

	ofVideoGrabber m_vidGrabber;
	bool firstFrame;
	
	//image frames
	ofxCvColorImage m_curImg;
	IplImage* m_oldFrame;
	IplImage* m_newFrame;

	//tracking vars
	int m_numFeatures;
	char* m_pointFound;
	CvPoint2D32f* m_oldImgFeatures;
	CvPoint2D32f* m_newImgFeatures;

	//for use in algorithms
	int m_flowFlags;
	CvTermCriteria m_termCriteria;
	IplImage* m_eig_image;
	IplImage* m_tmp_image;
	IplImage * m_pyramidNew;
	IplImage * m_pyramidOld;

	//vector field
	float m_fieldDensity;
	float m_fieldDensityInv;
	int m_fieldWidth;
	int m_fieldHeight;
	Particle* m_vectorField;
	float m_vectorFieldNorm;

public:
	OpenCVManager(vector<Particle*>* people, IScene** currentScene, float renderScale);

	void update(int deltaTime);
	void debugDraw();

	void learnBackground();

	~OpenCVManager();
};

