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
	std::cout << "Shutdown SoundManager";
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

void Sound::playSampleOnce(std::string fileName)
{
	osg::ref_ptr<osgAudio::SoundState> sound_state = osgAudio::SoundManager::instance()->findSoundState("single");
	if(!sound_state) 
	{
		sound_state = new osgAudio::SoundState("single");
		osgAudio::Sample *sample = new osgAudio::Sample(osgDB::findDataFile(fileName));
		sound_state->setSample(sample);
		sound_state->setLooping(false);
		sound_state->setAmbient(true);
		sound_state->setGain(0.5);
		sound_state->allocateSource(2);
		osgAudio::SoundManager::instance()->addSoundState(sound_state.get());
	}
	if(!sound_state->isPlaying())
		sound_state->setPlay(true);
}