#include "AudioManager.h"


AudioManager::AudioManager()
{
}

ofSoundPlayer* AudioManager::load(string filename)
{
	for (vector<SoundObj>::iterator so = m_loadedSounds.begin(); so != m_loadedSounds.end(); ++so)
	{
		if (so->filename == filename)
			return so->player;
	}

	SoundObj newSoundObj;
	newSoundObj.player = new ofSoundPlayer();
	newSoundObj.player->loadSound(filename);
	newSoundObj.filename = filename;
	m_loadedSounds.push_back(newSoundObj);
	return newSoundObj.player;
}

AudioManager::~AudioManager()
{
}
