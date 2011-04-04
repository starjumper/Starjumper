#include "Sound.h"

Sound *Sound::_instance = NULL;

Sound::Sound()
{
	alureInitDevice(NULL, NULL);
}

Sound *Sound::getInstance() 
{ 
    if (!_instance)
        _instance = new Sound();

    return _instance;
}

void Sound::loadFromFile(std::string filename)
{
	ALuint source;
	
	alGenSources(1, &source);
	alSourcei(source, AL_BUFFER, alureCreateBufferFromFile(filename.c_str()));
	
	_sounds[filename] = source;
}

void Sound::playSound(std::string key)
{
	alSourcei(_sounds[key], AL_LOOPING, AL_FALSE);
	alSourcePlay(_sounds[key]);
}

void Sound::playInLoop(std::string key)
{
	alSourcei(_sounds[key], AL_LOOPING, AL_TRUE);
	alSourcePlay(_sounds[key]);
}

void Sound::stop(std::string key)
{
	alSourceStop(_sounds[key]);
}

void Sound::stopAll()
{
	for (std::map<std::string, ALuint>::iterator i = _sounds.begin(); i != _sounds.end(); ++i)
		stop(i->first);
}

Sound::~Sound()
{
	alureShutdownDevice();
}