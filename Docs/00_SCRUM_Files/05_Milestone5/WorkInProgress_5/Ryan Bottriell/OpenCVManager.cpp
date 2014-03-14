#include "OpenCVManager.h"

static CvSize s_frameSize = cvSize(160, 120);
static ofVec2f s_frameSizeInv = ofVec2f(1.f/s_frameSize.width, 1.f/s_frameSize.height);
static int s_maxFeatures = 300;
static int s_vectorFieldDensity = 50;
static float s_vectorFieldDensityInv = 1.f/s_vectorFieldDensity;
static int s_generationBuffer = 100;
static int s_maxPeopleParticles = 15;

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
	m_vectorFieldNorm = m_fieldWidth * 0.01f;
	
	int count = 0;

	for (int i = 0; i < m_fieldWidth; ++i)
	{
		for (int j = 0; j < m_fieldHeight; ++j)
		{
			int pos = (i*m_fieldHeight) + j;
			m_vectorField[pos] = Particle(ofVec3f(i * s_vectorFieldDensity, j * s_vectorFieldDensity));
			m_vectorField[pos].vel = ofVec3f(m_vectorFieldNorm, 0.f);
			m_vectorField[pos].maxSpeed = s_vectorFieldDensity;
		}
	}

	crowdLastGenerated = -5000;
	lastFrame = 0;
}

void OpenCVManager::update(int deltaTime)
{
	int stepTime = lastFrame + deltaTime;

	///////////////////////////
	//  update vector field  //
	///////////////////////////

	//approach normal
	Particle* vect;
	for (int i = 0; i < m_fieldWidth; ++i)
	{
		for (int j = 0; j < m_fieldHeight; ++j)
		{
			vect = &m_vectorField[(i*m_fieldHeight) + j];

			ofVec3f target;
			if (vect->vel.lengthSquared() > 0.f)
				target = (vect->vel / vect->vel.length()) * m_vectorFieldNorm;
			else
				target = ofVec3f(m_vectorFieldNorm, 0.f);
			vect->vel += (target - vect->vel) * 0.01;
		}
	}

	//update from video
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

			//convert color
			cvCvtColor(m_curImg.getCvImage(), buff, CV_RGB2GRAY);

			//mirror image
			cvFlip(buff, m_newFrame, -1);

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

			//affect vector field
			ofVec2f deltaVec;
			Particle* tmpVec;

			for (int i = 0; i < m_numFeatures; ++i)
			{
				if (!m_pointFound[i])
					continue;

				deltaVec = ofVec2f(m_newImgFeatures[i].x - m_oldImgFeatures[i].x, m_newImgFeatures[i].y - m_oldImgFeatures[i].y);
				
				if (deltaVec.lengthSquared() < m_vectorFieldNorm * m_vectorFieldNorm)
					continue;

				//closest field value
				int posX = (int) m_newImgFeatures[i].x * s_frameSizeInv.x * ofGetWidth() * s_vectorFieldDensityInv;
				int posY = (int) m_newImgFeatures[i].y * s_frameSizeInv.y * ofGetHeight() * s_vectorFieldDensityInv;
				tmpVec = &m_vectorField[(posX * m_fieldHeight) + posY];
				
				//tmpVec->vel = (deltaVec - tmpVec->vel);
				tmpVec->vel += deltaVec * deltaTime * 0.05;

				tmpVec->vel.limit(tmpVec->maxSpeed);
			}
		}
	}

	//////////////////////
	//  simulate crowd  //
	//////////////////////

	//generate a person each 5 seconds
	if (stepTime - crowdLastGenerated > 2000
		&& pPeople->size() < s_maxPeopleParticles)
	{
		float y = ofGetWindowHeight();
		y = ofRandom(y * 0.25f, y * 0.75f);

		float x = -s_generationBuffer;
		if (ofRandom(1) > 0.5)
			x = ofGetWidth() + s_generationBuffer;

		Particle* p = ppCurrentScene[0]->addParticleOfProperType(
			ofVec2f(x, y)
			);
		
		p->maxSpeed = s_vectorFieldDensity * 0.1f;
		p->vel = ofVec2f( (x < 0) ? 0.5f : -0.5f, 0.f);
		crowdLastGenerated = stepTime;
	}

	//move people across screen or remove them
	for (vector<Particle*>::iterator p = pPeople->begin(); p != pPeople->end();)
	{
		ofVec3f targetVel = ofVec3f(0.f, 0.f);

		//calculate vector field that's close
		//only if theres a webcam
		if (m_vidGrabber.isInitialized())
		{
			int fieldX = (*p)->pos.x / s_vectorFieldDensity;
			int fieldY = (*p)->pos.y / s_vectorFieldDensity;

			if (fieldX < 1) fieldX = 1;
			else if (fieldX > m_fieldWidth - 3) fieldX = m_fieldWidth - 3;
			if (fieldY < 1) fieldY = 1;
			else if (fieldY > m_fieldHeight - 3) fieldY = m_fieldHeight - 3;

			for (int i = -2; i < 3; ++i)
			{
				for (int j = -2; j < 3; ++j)
				{
					int pos = ((fieldX + i) * m_fieldHeight) + (fieldY * j);
					targetVel += m_vectorField[pos].vel;
				}
			}
			targetVel *= 0.04f;
			(*p)->accel = (targetVel - (*p)->vel) * deltaTime * 0.01f;
		}
		else
		{
			targetVel = (*p)->vel;
		}

		(*p)->update();// stepTimeDelta;

		if ((*p)->pos.x > ofGetWindowWidth() + s_generationBuffer*1.5f
			|| (*p)->pos.x < -s_generationBuffer * 1.5f
			|| (*p)->pos.y > ofGetHeight() + s_generationBuffer * 1.5f
			|| (*p)->pos.y < -s_generationBuffer * 1.5f)
		{
			p = pPeople->erase(p);
		}
		else
		{
			++p;
		}
	}
	lastFrame = stepTime;

}

void OpenCVManager::debugDraw()
{
	if (firstFrame) return;

	ofSetColor(255);
	ofPushMatrix();
		ofTranslate(ofGetWidth() - m_curImg.getWidth() * 2.f, 0);
		ofScale(2.f, 2.f);

		m_curImg.mirror(false, true);
		m_curImg.draw(0, 0);
		m_curImg.mirror(false, true);

		ofSetColor(255, 0, 0);
		ofNoFill();
		ofVec2f tmpVec;
		for (int i = 0; i < m_numFeatures; ++i)
		{
			if (!m_pointFound[i])
				continue;
			tmpVec = ofVec2f(m_newImgFeatures[i].x - m_oldImgFeatures[i].x, m_oldImgFeatures[i].y - m_newImgFeatures[i].y);
			float len = tmpVec.length();

			ofPushMatrix();
			ofTranslate(m_oldImgFeatures[i].x, s_frameSize.height - m_oldImgFeatures[i].y);
				ofRotate( ofRadToDeg( atan2f( tmpVec.y, tmpVec.x ) ) );
				ofLine(0.f, 0.f, len * 2.f, 0.f);
			ofPopMatrix();
		}

	ofPopMatrix();

	ofPushMatrix();
		ofSetColor(180, 0, 0, 255);
		ofTranslate(s_vectorFieldDensity / 2, s_vectorFieldDensity / 2);

		Particle* vector;
		for (int i = 0; i < m_fieldWidth; ++i)
		{
			for (int j = 0; j < m_fieldHeight; ++j)
			{
				vector = &m_vectorField[ (i*m_fieldHeight) + j];
				float len = vector->vel.length();

				ofPushMatrix();
					ofTranslate(vector->pos);
					ofFill();
					ofRect(-2.f, -2.f, 2.f, 2.f);
					ofNoFill();
					ofRotate(ofRadToDeg(atan2f(vector->vel.y, vector->vel.x)));
					ofLine(0.f, 0.f, len * 10.f, 0.f);
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
