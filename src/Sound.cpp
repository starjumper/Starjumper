#include "Sound.h"

Sound::Sound() 
{
}

void Sound::initSoundManager()
{
	osgAudio::SoundManager::instance()->init(16, false);
	osgAudio::SoundManager::instance()->getEnvironment()->setDistanceModel(osgAudio::InverseDistance);
	osgAudio::SoundManager::instance()->getEnvironment()->setDopplerFactor(1);
}

void Sound::shutdownSoundManager()
{
	osgAudio::SoundManager::instance()->shutdown();
}

void Sound::switchBackgroundMusic(std::string fileName, std::string soundStateName)
{
	osgAudio::SoundState *musicSoundState = osgAudio::SoundManager::instance()->findSoundState(soundStateName);
	if (musicSoundState)
	{
		if(!musicSoundState->isPlaying()) 
		{
			osgAudio::SoundManager::instance()->stopAllSources();
			musicSoundState->setPlay(true);
		}
	}
	else
	{
		osgAudio::SoundManager::instance()->stopAllSources();
		musicSoundState = new osgAudio::SoundState(soundStateName);
		musicSoundState->allocateSource(1);

		osgAudio::FileStream *musicStream = new osgAudio::FileStream(fileName);

		musicSoundState->setStream(musicStream);
		musicSoundState->setAmbient(true);
		musicSoundState->setLooping(true);
		musicSoundState->setPlay(true);

		osgAudio::SoundManager::instance()->addSoundState(musicSoundState);
	}
}