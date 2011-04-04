#pragma once

#include <map>
#include <string>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alure.h>

class Sound
{
private:
	static Sound* instance;
	std::map<std::string, ALuint> sounds;
	Sound();

public:
	~Sound();
	
	static Sound *getInstance();

	void loadSoundFromFile(std::string key, std::string filename);
	void play(std::string key);
	void loop(std::string key);
	void stop(std::string key);
	void stopAll();
};
