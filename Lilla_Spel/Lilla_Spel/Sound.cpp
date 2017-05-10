#include "Sound.h"

SoundManager::SoundManager()
{
	this->amountOfSounds = 0;
	this->soundCap = 1;
	this->Reverb = new FMOD::Reverb3D*[this->soundCap];
	this->Reverb[0] = nullptr;

	this->prop = new FMOD_REVERB_PROPERTIES[1];
	this->prop2 = new FMOD_REVERB_PROPERTIES[1];

	prop->DecayTime = 1500;
	prop->Density = 7;
	prop->Diffusion = 11;
	prop->EarlyDelay = 5000;
	prop->EarlyLateMix = 54;
	prop->HFDecayRatio = 100;
	prop->HFReference = 60;
	prop->HighCut = 250;
	prop->LateDelay = 0;
	prop->LowShelfFrequency = 2900;
	prop->LowShelfGain = 83;
	prop->WetLevel = 0.5f;

}
SoundManager::~SoundManager()
{
	this->Clean();
}

void SoundManager::createFMOD()
{
	FMOD_RESULT fr;
	fr = FMOD::System_Create(&system);      // Create the main system object.
	if (fr != FMOD_OK)
	{
		std::cout << ("FMOD error! (%d) %s\n", fr, FMOD_ErrorString(fr));
		exit(-1);
	}

	fr = system->init(512, FMOD_INIT_NORMAL, 0);    // Initialize FMOD.
	if (fr != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", fr, FMOD_ErrorString(fr));
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
	
	Reverb[0]->setProperties(prop);
	Reverb[0]->getProperties(prop2);
	std::cout << 1 << " " << prop2->DecayTime<< std::endl;

	this->prop->DecayTime += 500;

	std::cout << 2 << " " << prop2->DecayTime << std::endl;

	Reverb[1]->setProperties(prop);
	Reverb[1]->getProperties(prop3);

	std::cout << 3 << " " << prop3->DecayTime << std::endl;

	std::cout << std::endl << std::endl;

}
void SoundManager::expandReverb()
{
	this->soundCap += 20;
	FMOD::Reverb3D** tempReverb = new FMOD::Reverb3D*[this->soundCap];

	for (int i = 0; i < this->amountOfSounds; i++)
	{
		tempReverb[i] = this->Reverb[i];
	}

	this->Reverb = tempReverb;
}
