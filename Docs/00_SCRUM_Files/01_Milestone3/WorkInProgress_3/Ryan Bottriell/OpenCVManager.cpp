#include "OpenCVManager.h"


OpenCVManager::OpenCVManager(vector<Particle*>* people, IScene** currentScene)
{
	pPeople = people;
	ppCurrentScene = currentScene;

	m_vidGrabber.setVerbose(true);
	m_vidGrabber.initGrabber(320, 240);

	//for tracking
	m_detector = cv::SurfFeatureDetector();
	m_keyPoints = vector<cv::KeyPoint>();
	//tracking vars
	/*m_corners = new CvPoint2D32f[0];
	m_numCorners = 0;
	m_cornerQualityLevel = 0.1;
	m_minCornerDistance = sqrt(240 * 480) * 0.05;*/

	//my vars
	learnBG = true;
	diffThresh = 50;

#ifdef SIMULATE_CROWD
	crowdLastGenerated = -5000;
	lastFrame = 0;
#endif


}

void OpenCVManager::update(int deltaTime)
{
	int stepTime = lastFrame + deltaTime;

#ifdef SIMULATE_CROWD

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

#endif

	if (m_vidGrabber.isInitialized())
	{
		bool newFrame = false;
		int id = 0;

		m_vidGrabber.update();
		newFrame = m_vidGrabber.isFrameNew();

		if (newFrame)
		{
			m_prevImg = m_curImg;
			m_curImg.setFromPixels(m_vidGrabber.getPixels(), 320, 240);

			//setup first frame
			if (learnBG)
			{
				learnBG = false;
			}
			
			//detect points
			m_detector.detect(m_curImg.getCvImage(), m_keyPoints);
		}
	}

}

void OpenCVManager::debugDraw()
{
	ofSetColor(255);
	ofPushMatrix();

	ofTranslate(ofGetWidth() - m_curImg.getWidth(), 0);

	m_curImg.draw(0, 0);

	ofSetColor(255, 0, 0);
	for(vector<cv::KeyPoint>::iterator kp = m_keyPoints.begin(); kp != m_keyPoints.end(); ++kp)
	{
		ofPushMatrix();
			ofTranslate( kp->pt.x, kp->pt.y );
			ofEllipse(0.f, 0.f, 0.f, 10.f, 10.f);
		ofPopMatrix();
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
