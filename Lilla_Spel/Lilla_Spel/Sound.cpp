#include "Sound.h"
#include <d3d11.h>

SoundManager::SoundManager()
{
	this->amountOfSounds = 0;
	this->soundCap = 1;
	this->Reverb = new FMOD::Reverb3D*[this->soundCap];
	this->Reverb[0] = nullptr;

	this->prop = new FMOD_REVERB_PROPERTIES*[3];

	for (int i = 0; i < 3; i++)
	{
		this->prop[i] = new FMOD_REVERB_PROPERTIES;
	}

	prop[0]->DecayTime = 1500;
	prop[0]->Density = 7;
	prop[0]->Diffusion = 11;
	prop[0]->EarlyDelay = 5000;
	prop[0]->EarlyLateMix = 54;
	prop[0]->HFDecayRatio = 100;
	prop[0]->HFReference = 60;
	prop[0]->HighCut = 250;
	prop[0]->LateDelay = 0;
	prop[0]->LowShelfFrequency = 2900;
	prop[0]->LowShelfGain = 83;
	prop[0]->WetLevel = 0.5f;

	this->soundList = new FMOD::Sound*[4];
	this->soundChannel = new FMOD::Channel*[4];

	this->soundChannel[0]->setVolume(0.0001f);
	this->soundChannel[3]->setVolume(2.0f);

}
SoundManager::~SoundManager()
{
	this->Clean();
}

void SoundManager::playSound(int i)
{
	this->system->playSound(this->soundList[i], 0, false, &this->soundChannel[i]);
}
void SoundManager::stopSound(int i)
{
	this->soundChannel[i]->stop();
}
void SoundManager::togglePauseSound(int i, bool paused)
{
	this->soundChannel[i]->setPaused(paused);
}
void SoundManager::setVolume(int i, float volume)
{
	this->soundChannel[i]->setVolume(volume);
}

void SoundManager::createFMOD()
{
	FMOD_RESULT fr;
	fr = FMOD::System_Create(&system);      // Create the main system object.
	if (fr != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", fr, FMOD_ErrorString(fr));
		getchar();
		exit(-1);
	}

	fr = system->init(512, FMOD_INIT_NORMAL, 0);    // Initialize FMOD.
	if (fr != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", fr, FMOD_ErrorString(fr));
		getchar();
		exit(-1);
	}

	fr = system->createSound(".\\Assets\\Sound\\Theme.mp3", FMOD_2D, 0, &this->soundList[0]);
	if (fr != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", fr, FMOD_ErrorString(fr));
		getchar();
		exit(-1);
	}

	fr = system->createSound(".\\Assets\\Sound\\Blop.mp3", FMOD_2D, 0, &this->soundList[1]);
	if (fr != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", fr, FMOD_ErrorString(fr));
		getchar();
		exit(-1);
	}

	fr = system->createSound(".\\Assets\\Sound\\Crumbling.mp3", FMOD_2D, 0, &this->soundList[2]);
	if (fr != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", fr, FMOD_ErrorString(fr));
		getchar();
		exit(-1);
	}

	fr = system->createSound(".\\Assets\\Sound\\Lever.mp3", FMOD_2D, 0, &this->soundList[3]);
	if (fr != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", fr, FMOD_ErrorString(fr));
		getchar();
		exit(-1);
	}

}
void SoundManager::update()
{
	FMOD_RESULT fr;
	fr = this->system->update();
	if (FAILED(fr))
	{
		return exit(-1);
	}
}

void SoundManager::addReverb(int addedReverb)
{
	if (this->amountOfSounds >= this->soundCap)
	{
		this->expandReverb();
	}

	int tempNrReverbs = this->amountOfSounds + addedReverb;

	for (int i = this->amountOfSounds; i < tempNrReverbs; i++)
	{
	this->system->createReverb3D(&this->Reverb[i]);
	this->amountOfSounds++;
	}
	
	this->amountOfSounds += addedReverb;

	std::cout << std::endl << std::endl;
	
	Reverb[0]->setProperties(prop[0]);
	Reverb[0]->getProperties(prop[1]);
	std::cout << 1 << " " << prop[1]->DecayTime<< std::endl;
	
	prop[0]->DecayTime += 200;

	Reverb[3]->setProperties(prop[0]);
	Reverb[3]->getProperties(prop[2]);
	std::cout << 1 << " " << prop[2]->DecayTime << std::endl;
}
void SoundManager::expandReverb()
{
	FMOD::Reverb3D** tempReverb = new FMOD::Reverb3D*[this->soundCap + 20];

	for (int i = 0; i < this->amountOfSounds; i++)
	{
		tempReverb[i] = this->Reverb[i];
	}
	this->Reverb = tempReverb;

	this->soundCap += 20;
}
