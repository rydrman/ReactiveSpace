#include "OpenCVManager.h"

static CvSize s_frameSize = cvSize(160, 120);
static int s_maxFeatures = 300;
static int s_vectorFieldDensity = 50;

OpenCVManager::OpenCVManager(vector<Particle*>* people, IScene** currentScene)
{
	pPeople = people;
	ppCurrentScene = currentScene;

	m_vidGrabber.setVerbose(true);
	m_vidGrabber.initGrabber(s_frameSize.width, s_frameSize.height);
	firstFrame = true;

	//tracking vars
	m_flowFlags = 0;
	m_termCriteria = cvTermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 50, 0.1);
	m_oldImgFeatures = new CvPoint2D32f[s_maxFeatures];
	m_newImgFeatures = new CvPoint2D32f[s_maxFeatures];

	m_oldFrame = cvCreateImage(s_frameSize, IPL_DEPTH_8U, 1);
	m_newFrame = cvCreateImage(s_frameSize, IPL_DEPTH_8U, 1);
	m_eig_image = cvCreateImage(s_frameSize, IPL_DEPTH_8U, 1);
	m_tmp_image = cvCreateImage(s_frameSize, IPL_DEPTH_8U, 1);
	m_pyramidNew = cvCreateImage(s_frameSize, IPL_DEPTH_8U, 1);
	m_pyramidOld = cvCreateImage(s_frameSize, IPL_DEPTH_8U, 1);

	//my vars
	learnBG = true;
	diffThresh = 50;

	m_fieldWidth = ofGetWidth() / s_vectorFieldDensity;
	m_fieldHeight = ofGetHeight() / s_vectorFieldDensity;
	m_vectorField = new Particle[m_fieldWidth * m_fieldHeight];
	
	for (int i = 0; i < m_fieldWidth; ++i)
	{
		for (int j = 0; j < m_fieldHeight; ++j)
		{
			m_vectorField[i*j] = Particle(ofVec3f(i * s_vectorFieldDensity, j * s_vectorFieldDensity));
			cout << i * s_vectorFieldDensity << ", " << j * s_vectorFieldDensity << endl;
		}
	}

	crowdLastGenerated = -5000;
	lastFrame = 0;
}

void OpenCVManager::update(int deltaTime)
{
	int stepTime = lastFrame + deltaTime;

	////////////////////////////
	//  first simulate crowd  //
	//////////////////////////// 

	//generate a person each 5 seconds
	if (stepTime - crowdLastGenerated > 5000)
	{
		int winH = ofGetWindowHeight();
		Particle* p = ppCurrentScene[0]->addParticleOfProperType(
			ofVec2f(0.f, ofRandom(winH * 0.25, winH * 0.75))
			);

		p->vel = ofVec2f(ofRandom(0.3f, 1.f), 0.f);
		crowdLastGenerated = stepTime;
	}

	//move people across screen or remove them
	for (vector<Particle*>::iterator p = pPeople->begin(); p != pPeople->end();)
	{
		(*p)->update();// stepTimeDelta;

		if ((*p)->pos.x > ofGetWindowWidth())
		{
			p = pPeople->erase(p);
		}
		else
		{
			++p;
		}
	}
	lastFrame = stepTime;

	////////////////////////////////
	//  then update vector field  //
	////////////////////////////////

	if (m_vidGrabber.isInitialized())
	{
		bool isNewFrame = false;
		int id = 0;

		m_vidGrabber.update();
		isNewFrame = m_vidGrabber.isFrameNew();

		if (isNewFrame)
		{
			//grab new frame
			m_curImg.setFromPixels(m_vidGrabber.getPixels(), s_frameSize.width, s_frameSize.height);

			//populate image
			if (firstFrame)
			{
				cvCvtColor(m_curImg.getCvImage(), m_newFrame, CV_RGB2GRAY);;
			}

			//cycle new to old
			IplImage* buff = m_pyramidOld;
			m_pyramidOld = m_pyramidNew;
			m_pyramidNew = buff;
			buff = m_oldFrame;
			m_oldFrame = m_newFrame;
			m_newFrame = buff;

			m_numFeatures = s_maxFeatures;

			cvCvtColor(m_curImg.getCvImage(), m_newFrame, CV_RGB2GRAY);

			cvGoodFeaturesToTrack(m_oldFrame, m_eig_image, m_tmp_image, m_oldImgFeatures, &m_numFeatures, 0.02, 1.0);

			//run flow
			int level = 2;// num levels in pyramid
			m_pointFound = new char[m_numFeatures];
			float* err = new float[s_maxFeatures];

			cvCalcOpticalFlowPyrLK(m_oldFrame, m_newFrame, 
				m_pyramidOld, m_pyramidNew, 
				m_oldImgFeatures, m_newImgFeatures, m_numFeatures, 
				s_frameSize, level, m_pointFound, err, m_termCriteria, 0);

			//set flags if necessary
			if (firstFrame)
			{
				m_flowFlags = CV_LKFLOW_PYR_A_READY;
				firstFrame = false;
			}
		}
	}

}

void OpenCVManager::debugDraw()
{
	if (firstFrame) return;

	ofSetColor(255);
	ofPushMatrix();
		ofTranslate(ofGetWidth() - m_curImg.getWidth() * 2.f, 0);
		ofScale(2.f, 2.f);

		m_curImg.draw(0, 0);

		ofSetColor(255, 0, 0);
		ofNoFill();
		ofVec2f tmpVec;
		for (int i = 0; i < m_numFeatures; ++i)
		{
			if (!m_pointFound[i])
				continue;
			tmpVec = ofVec2f(m_newImgFeatures[i].x - m_oldImgFeatures[i].x, m_newImgFeatures[i].y - m_oldImgFeatures[i].y);
			float len = tmpVec.length();

			ofPushMatrix();
				ofTranslate(m_oldImgFeatures[i].x, m_oldImgFeatures[i].y);
				ofRotate( ofRadToDeg( atan2f( tmpVec.y, tmpVec.x ) ) );
				ofLine(0.f, 0.f, len * 2.f, 0.f);
			ofPopMatrix();
		}

	ofPopMatrix();

	ofPushMatrix();
		ofFill();
		ofSetColor(255, 255, 0, 255);

		for (int i = 0; i < m_fieldWidth; ++i)
		{
			for (int j = 0; j < m_fieldHeight; ++j)
			{
				ofPushMatrix();
					ofTranslate(m_vectorField[i*j].pos);
					ofRect(0, 0, 20.f, 20.f);
				ofPopMatrix();
			}
		}
	ofPopMatrix();
}

void OpenCVManager::learnBackground()
{
	learnBG = true;
}

OpenCVManager::~OpenCVManager()
{
	m_vidGrabber.close();
}
