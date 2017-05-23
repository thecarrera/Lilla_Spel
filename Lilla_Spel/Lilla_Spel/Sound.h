#pragma once

#include <fmod.hpp>
#include <fmod_errors.h>
#include <iostream>

class SoundManager
{
private:
	FMOD::System* system = nullptr;
	FMOD::Reverb3D** Reverb;
	FMOD_REVERB_PROPERTIES** prop = nullptr;

	FMOD::Sound** soundList = nullptr;
	FMOD::ChannelGroup* soundGroup;

	int amountOfSounds;
	int soundCap;

public:
	FMOD::Channel** soundChannel = nullptr;

public:
	SoundManager();
	~SoundManager();
	
	void playSound(int index);
	/*	MUST BE SET AFTER PLAYING A SOUND!
		
		Between 0->1
		Over 1 amplifies
		Under -0 -> -1 inverts the sound 
	*/
	void setVolume(int index, float volume); 
	void stopSound(int index);
	void togglePauseSound(int index, bool state);

	void playAllAmbient();
	void pauseAllAmbient(bool state);
	void stopAllAmbient();

	void createFMOD();
	void loadSound();
	void update();

	void addReverb(int i); 
	void expandReverb();
	void placeReverbs();

	void setListnerPos(FMOD_VECTOR pos);
	
	void Clean() { system->release(); };
	
};