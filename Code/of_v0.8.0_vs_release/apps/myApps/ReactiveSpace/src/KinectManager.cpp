#include "KinectManager.h"

KinectManager::KinectManager(vector<Particle*>* hands, IScene** currentScene)
{
	kinectInitialized = false;
	kinectFailed = false;

	ppCurrentScene = currentScene;
	pHandPositions = hands;
	handParticleTimeout = 1000;

	pSkeletonFrame = new NUI_SKELETON_FRAME();
	colorStream = HANDLE();
}

HRESULT KinectManager::initialize()
{
	// reset vars //
	kinectInitialized = false;
	kinectFailed = false;

	// create vars //
	int numSensors;
	
	//get number of sensors
	HRESULT hr = NuiGetSensorCount(&numSensors);

	if (SUCCEEDED(hr))
	{
		if (numSensors == 0)
		{
			//NO KINECTS CONNECTED
			hr = MAKE_HRESULT(0, 0, -1);
			kinectFailed = true;
			
			cout << "No Kinect sensors found\n";
		}
		else
		{
			cout << "Found sensor...\n";
			//get the first kinect and its status
			NuiCreateSensorByIndex(0, &pKinect);
			hr = pKinect->NuiStatus();
		}
	}

	if (SUCCEEDED(hr))
	{
		//kinect ready
		cout << "Kinect is ready, starting now\n";
		kinectInitialized = true;
		startKinect();
	}
	else if (hr != S_NUI_INITIALIZING)
	{
		kinectFailed = true;
		cout << "Kinect failed and is not initializing\n";
	}

	return hr;
}

HRESULT KinectManager::startKinect()
{
	HRESULT hr = pKinect->NuiInitialize( NUI_INITIALIZE_FLAG_USES_SKELETON | NUI_INITIALIZE_FLAG_USES_COLOR );

	if (SUCCEEDED(hr))
	{
		hr = pKinect->NuiSkeletonTrackingEnable(NULL, NUI_SKELETON_TRACKING_FLAG_ENABLE_SEATED_SUPPORT);
	}

	if (SUCCEEDED(hr))
	{
		cout << "Kinect skeletons successfully initialized\n";

		//hr = pKinect->NuiImageStreamOpen(NUI_IMAGE_TYPE_COLOR, NUI_IMAGE_RESOLUTION_320x240, NUI_IMAGE_STREAM_FLAG_SUPPRESS_NO_FRAME_DATA, INFINITE, NULL, &colorStream);
	}

	if (FAILED(hr))
		cout << "Kinect couldn't initialize\n";
	//else
		//cout << "Kinect color stream successfully initialized\n";

	return hr;
}

HRESULT KinectManager::update(float timeScale)
{
	//first add time to particles and remove them if they're dead
	try{
		pHandPositions->size();
	}
	catch (exception e)
	{
		cout << e.what() << endl;
		return MAKE_HRESULT(0, 0, -1);
	}
	for (vector<Particle*>::iterator hp = pHandPositions->begin(); hp != pHandPositions->end();)
	{
		(*hp)->timer += 16.f / timeScale;

		if ((*hp)->timer > handParticleTimeout)
			hp = pHandPositions->erase(hp);
		else
			++hp;
	}

	if (kinectFailed)
		return MAKE_HRESULT(0, 0, -1);
	
	HRESULT hr;
	if (!kinectInitialized)
	{
		hr = pKinect->NuiStatus();

		if (hr == S_NUI_INITIALIZING)
		{
			cout << "kinect is still initializing...\n";
			return hr;
		}
		else if (FAILED(hr))
		{
			kinectFailed = true;
			return hr;
		}

		//kinect ready, initialize
		kinectInitialized = true;
		startKinect();
	}
	
	// kinect if okay, do updating //

	//get next frame
	hr = pKinect->NuiSkeletonGetNextFrame(5, pSkeletonFrame);

	if (SUCCEEDED(hr))
	{
		//pHandPos->clear();
		ofVec3f pos;
		for (int i = 0; i < NUI_SKELETON_COUNT; i++)
		{
			//if its a tracked skeleton
			if (pSkeletonFrame->SkeletonData[i].eTrackingState == NUI_SKELETON_TRACKED)
			{
				//left hand
				pos = handPositionToScreenPosition(
					pSkeletonFrame->SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT]
					);
				addUpdate(pos, pSkeletonFrame->SkeletonData[i].dwTrackingID, NUI_SKELETON_POSITION_HAND_LEFT);

				pos = handPositionToScreenPosition(
					pSkeletonFrame->SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT]
					);
				addUpdate(pos, pSkeletonFrame->SkeletonData[i].dwTrackingID, NUI_SKELETON_POSITION_HAND_RIGHT);
			}
		}
	}
	else if (hr == E_NUI_FRAME_NO_DATA || hr == S_FALSE)
	{
		//new frame was not yet available
		//if (hr == E_NUI_FRAME_NO_DATA)
			//cout << "no Kinect Data" << ofGetFrameNum() << endl;
		return MAKE_HRESULT(0, 0, 1);
	}

	//NUI_IMAGE_FRAME nextFrame;
	//hr = pKinect->NuiImageStreamGetNextFrame(colorStream, 20, &nextFrame);

	return hr;
}

void KinectManager::addUpdate(ofVec3f pos, int trackingID, int jointIndex)
{

	//find matching particle
	vector<Particle*>::iterator pI;
	for (pI = pHandPositions->begin(); pI != pHandPositions->end(); ++pI)
	{
		if ((*pI)->ID == trackingID && (*pI)->jointIndex == jointIndex)
			break;
	}

	//update it or create a new one
	Particle* p;
	if (pI != pHandPositions->end())
	{
		p = (*pI);
		p->vel = (pos - p->pos) * 0.5f;
		p->timer = 0;
		p->update();
	}
	else
	{
		p = (*ppCurrentScene)->addHandOfProperType(pos);
		p->ID = trackingID;
		p->jointIndex = jointIndex;
		p->timer = 0;
		pHandPositions->push_back(p);
	}
}

ofVec3f KinectManager::handPositionToScreenPosition(Vector4 _pos)
{
	ofVec3f pos = ofVec3f();

	float xPos;
	float yPos;
	NuiTransformSkeletonToDepthImage(_pos, &xPos, &yPos, NUI_IMAGE_RESOLUTION_1280x960);

	pos.x = xPos / 1280 * ofGetWidth();
	pos.y = yPos / 960 * ofGetHeight();

	return pos;
}

bool KinectManager::isFailed()
{
	return kinectFailed;
}


KinectManager::~KinectManager()
{

}
