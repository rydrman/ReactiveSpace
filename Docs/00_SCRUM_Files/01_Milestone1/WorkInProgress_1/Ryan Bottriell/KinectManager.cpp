#include "KinectManager.h"
#include <NuiApi.h>


KinectManager::KinectManager()
{
	kinectInitialized = false;
	kinectFailed = false;

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

HRESULT KinectManager::update(int dt, vector<Vector4>* pHandPos)
{
	if (kinectFailed)
		return MAKE_HRESULT(0, 0, -1);
	
	HRESULT hr;
	if (!kinectInitialized)
	{
		hr = pKinect->NuiStatus();

		if (hr == S_NUI_INITIALIZING)
		{
			cout << "kinect is stil initializing...\n";
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
	hr = pKinect->NuiSkeletonGetNextFrame(10, pSkeletonFrame);

	if (SUCCEEDED(hr))
	{
		pHandPos->clear();
		for (int i = 0; i < NUI_SKELETON_COUNT; i++)
		{
			//if its a tracked skeleton
			if (pSkeletonFrame->SkeletonData[i].eTrackingState == NUI_SKELETON_TRACKED)
			{
				//found skeleton, push hand positions
				pHandPos->push_back(
					handPositionToScreenPosition(
					pSkeletonFrame->SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT]
					));
				pHandPos->push_back(
					handPositionToScreenPosition(
					pSkeletonFrame->SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT]
					));
			}
		}
	}

	NUI_IMAGE_FRAME nextFrame;
	hr = pKinect->NuiImageStreamGetNextFrame(colorStream, 20, &nextFrame);

	if (SUCCEEDED(hr))
	{
		//get image stream

	}

	return hr;
}

Vector4 KinectManager::handPositionToScreenPosition(Vector4 pos)
{
	float xPos;
	float yPos;
	NuiTransformSkeletonToDepthImage(pos, &xPos, &yPos, NUI_IMAGE_RESOLUTION_1280x960);

	pos.x = xPos / 1280 * ofGetScreenWidth();
	pos.y = yPos / 960 * ofGetScreenHeight();

	return pos;
}


KinectManager::~KinectManager()
{

}
