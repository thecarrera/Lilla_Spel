#pragma once

#include "Includes.h"

class SoundManager
{
private:
	FMOD::System* system = nullptr;
	FMOD::Reverb3D** Reverb;
	FMOD_REVERB_PROPERTIES* prop;
	FMOD_REVERB_PROPERTIES* prop2;
	FMOD_REVERB_PROPERTIES* prop3;
	FMOD_REVERB_PROPERTIES* prop4;

	int amountOfSounds;
	int soundCap;

public:
	SoundManager();
	~SoundManager();

	void createFMOD();
	void update();

	void addReverb(int i); 
	void expandReverb();
	void addSound();


	void Clean() { system->release(); };

};