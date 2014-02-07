#pragma once
#include "ofMain.h"
#include <Windows.h>
#include <Ole2.h>
#include <NuiApi.h>

class KinectManager
{
	bool kinectInitialized;
	bool kinectFailed;

	INuiSensor* pKinect;

	NUI_SKELETON_FRAME* pSkeletonFrame;

	HANDLE colorStream;

public:
	KinectManager();

	HRESULT initialize();
	HRESULT startKinect();

	HRESULT update(int dt, vector<Vector4>* pHandPos);

	Vector4 handPositionToScreenPosition(Vector4 pos);

	bool isFailed();

	~KinectManager();
};

