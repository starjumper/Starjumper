#pragma once

#include <osgDB/FileUtils>
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
		static void playSampleOnce(std::string fileName);
};