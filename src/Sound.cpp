#include <Sound.h>

Sound* Sound::instance = new Sound();

Sound::Sound()
{
	alureInitDevice(NULL, NULL);
}

Sound::~Sound()
{
	alureShutdownDevice();
}

Sound* Sound::getInstance()
{
	return Sound::instance;
}

void Sound::loadSoundFromFile(std::string key, std::string filename)
{
	ALuint source;
	
	alGenSources(1, &source);
	
	alSourcei(source, AL_BUFFER, alureCreateBufferFromFile((ALchar*)filename.c_str()));
	
	sounds[key] = source;
}

void Sound::play(std::string key)
{
	alSourcei(sounds[key], AL_LOOPING, AL_FALSE);
	
	alSourcePlay(sounds[key]);
}

void Sound::loop(std::string key)
{
	alSourcei(sounds[key], AL_LOOPING, AL_TRUE);
	
	alSourcePlay(sounds[key]);
}

void Sound::stop(std::string key)
{
	alSourceStop(sounds[key]);
}

void Sound::stopAll()
{
	for (std::map<std::string, ALuint>::iterator i = sounds.begin(); i != sounds.end(); ++i)
	{
		stop(i->first);
	}
}