#pragma once
#include "ofMain.h"

#include <Windows.h>
#include <Ole2.h>
#include <NuiApi.h>

#include "ParticleMain.h"
#include "IScene.h"



class KinectManager
{
	vector<Particle*>* pHandPositions;
	IScene** ppCurrentScene;

	bool kinectInitialized;
	bool kinectFailed;

	INuiSensor* pKinect;

	NUI_SKELETON_FRAME* pSkeletonFrame;

	HANDLE colorStream;

	int handParticleTimeout;

public:
	KinectManager(vector<Particle*>* hands, IScene** currentScene);

	HRESULT initialize();
	HRESULT startKinect();

	HRESULT update(float timeScale);

	void addUpdate(ofVec3f pos, int trackingID, int jointIndex);

	ofVec3f handPositionToScreenPosition(Vector4 pos);

	bool isFailed();

	~KinectManager();
};

