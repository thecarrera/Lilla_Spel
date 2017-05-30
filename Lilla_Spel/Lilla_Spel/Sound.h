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

	time_t tButtonPress;
	time_t lTimePress;

private:
	bool faze1 = false;
	bool faze2 = false;
	bool enteredSecret = false;
	bool enteredCave = false;
	bool ending = false;

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
	
	void checkUnique(float posX, float posZ);
	void checkBear(float posX, float posZ);
private:
	void checkIndian(float posX, float posZ);
	void checkEnteringCave(float posX, float posZ);
	void checkExitingCave(float posX, float posZ);
	void checkEnding(float posX, float posZ);

};