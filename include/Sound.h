#pragma once

#include <osgAudio/FileStream.h>
#include <osgAudio/SoundManager.h>
#include <osgAudio/SoundState.h>
#include <osgAudio/Stream.h>

class Sound
{
	public:
		Sound();
		static void initSoundManager();
		static void shutdownSoundManager();
		static void switchBackgroundMusic(std::string fileName, std::string soundStateName);
};