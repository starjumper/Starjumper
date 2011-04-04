#pragma once

#include <map>
#include <string>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alure.h>

#define LEVEL_MUSIC_FILE        "resources/sound/andromeda.wav"
#define MENU_MUSIC_FILE         "resources/sound/48000_2chan.wav"
#define JUMP_SOUND              "resources/sound/boing.wav"

class Sound
{
private:
	static Sound *_instance;
	std::map<std::string, ALuint> _sounds;

	Sound();

public:
	~Sound();
	
	static Sound *getInstance();

	void loadFromFile(std::string filename);
	void playSound(std::string key);
	void playInLoop(std::string key);
	void stop(std::string key);
	void stopAll();
};
