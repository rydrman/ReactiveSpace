#pragma once
#include "ofMain.h"

using std::vector;

struct SoundObj
{
	ofSoundPlayer* player;
	string filename;
};

class AudioManager
{
private:
	vector<SoundObj> m_loadedSounds;

public:
	AudioManager();

	ofSoundPlayer* load(string filename);

	~AudioManager();
};

