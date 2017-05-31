#include "Sound.h"
#include <d3d11.h>
#include <mutex>

SoundManager::SoundManager()
{
	this->amountOfSounds = 0;
	this->soundCap = 1;
	this->Reverb = new FMOD::Reverb3D*[this->soundCap];
	this->Reverb[0] = nullptr;

	this->prop = new FMOD_REVERB_PROPERTIES*[this->soundCap];

	for (int i = 0; i < this->soundCap; i++)
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

	this->soundList = new FMOD::Sound*[11];
	this->soundChannel = new FMOD::Channel*[17];

	/*
	########################################
	#				  Crows				   #
	########################################
	*/
	FMOD_VECTOR tempPos = { 111.339f, 0.0f, 42.0001f };
	FMOD_VECTOR tempVel = { 0.8f, 0.9f, 0.9f };
	this->soundChannel[1]->set3DAttributes(&tempPos, &tempVel, 0);

	///*
	//########################################
	//#				  Cave				   #
	//########################################
	//*/

	//tempPos = { 1171.41f, 0.0f, 47.6001f };
	//this->soundChannel[2]->set3DAttributes(&tempPos, &tempVel, 0);

	///*
	//########################################
	//#				  Water				   #
	//########################################
	//*/

	//tempPos = { 111.339f, 0.0f, 42.0001f };
	//this->soundChannel[3]->set3DAttributes(&tempPos, &tempVel, 0);

	//tempPos = { 176.798f, 0.0f, 35.6001f };
	//this->soundChannel[3]->set3DAttributes(&tempPos, &tempVel, 0);

	//tempPos = { 221.998f, 0.0f, 78.9999f };
	//this->soundChannel[3]->set3DAttributes(&tempPos, &tempVel, 0);

	///*
	//########################################
	//#				  Swamp				   #
	//########################################
	//*/

	//tempPos = { 1788.06f, 0.0f, 36.6001f };
	//this->soundChannel[3]->set3DAttributes(&tempPos, &tempVel, 0);

	//tempPos = { 1762.46f, 0.0f, 142.199f };
	//this->soundChannel[3]->set3DAttributes(&tempPos, &tempVel, 0);

	//tempPos = { 1630.9f, 0.0f, 140.599f };
	//this->soundChannel[3]->set3DAttributes(&tempPos, &tempVel, 0);

	//tempPos = { 1633.3f, 0.0f, 40.4001f };
	//this->soundChannel[3]->set3DAttributes(&tempPos, &tempVel, 0);

	///*
	//########################################
	//#				 Easter			       #
	//########################################
	//*/

	//tempPos = { 1284.18f, 0.0f, 59.4002f };
	//this->soundChannel[3]->set3DAttributes(&tempPos, &tempVel, 0);

	///*
	//########################################
	//#				Cheering			   #
	//########################################
	//*/

	//tempPos = { 1960.82f, 0.0f, 19.0f };
	//this->soundChannel[3]->set3DAttributes(&tempPos, &tempVel, 0);

}
SoundManager::~SoundManager()
{
	this->Clean();
}
void SoundManager::Clean()
{
	for (int i = 0; i < this->soundCap; i++)
	{
		SAFE_RELEASE(this->Reverb[i]);
	} SAFE_DELETE(this->Reverb);

	SAFE_DELETE(this->Reverb);

	for (int i = 0; i < 11; i++)
	{
		SAFE_RELEASE(this->soundList[i]);
	} SAFE_DELETE(this->soundList);

	SAFE_DELETE(this->soundChannel);

	SAFE_DELETE(this->soundGroup);

	SAFE_RELEASE(this->system);

}

void SoundManager::playSound(int i)
{
	if (i == 0)
	{
		this->system->playSound(this->soundList[i], 0, false, &this->soundChannel[0]);
	}
	else if (i == 1)
	{
		this->system->playSound(this->soundList[i], 0, false, &this->soundChannel[1]);
	}
	else if (i == 2)
	{
		this->system->playSound(this->soundList[i], 0, false, &this->soundChannel[2]);
	}
	else if (i == 3)
	{
		this->system->playSound(this->soundList[i], 0, false, &this->soundChannel[3]);
		this->system->playSound(this->soundList[i], 0, false, &this->soundChannel[4]);
		this->system->playSound(this->soundList[i], 0, false, &this->soundChannel[5]);
		this->system->playSound(this->soundList[i], 0, false, &this->soundChannel[6]);
		this->system->playSound(this->soundList[i], 0, false, &this->soundChannel[7]);
		this->system->playSound(this->soundList[i], 0, false, &this->soundChannel[8]);
		this->system->playSound(this->soundList[i], 0, false, &this->soundChannel[9]);
	}
	else if(i == 4)
	{
		this->system->playSound(this->soundList[i], 0, false, &this->soundChannel[10]);
	}
	else if (i == 5)
	{
		this->system->playSound(this->soundList[i], 0, false, &this->soundChannel[11]);
	}
	else if (i == 6)
	{
		this->system->playSound(this->soundList[i], 0, false, &this->soundChannel[12]);
	}
	else if (i == 7)
	{
		this->system->playSound(this->soundList[i], 0, false, &this->soundChannel[13]);
	}
	else if (i == 8)
	{
		this->system->playSound(this->soundList[i], 0, false, &this->soundChannel[14]);
	}
	else if (i == 9)
	{
		this->system->playSound(this->soundList[i], 0, false, &this->soundChannel[15]);
	}
	else if (i == 10)
	{
		this->system->playSound(this->soundList[i], 0, false, &this->soundChannel[16]);
	}

}
void SoundManager::setVolume(int i, float volume)
{
	if (i == 0)
	{
		this->soundChannel[0]->setVolume(volume);
	}
	else if (i == 1)
	{
		this->soundChannel[1]->setVolume(volume);
	}
	else if (i == 2)
	{
		this->soundChannel[2]->setVolume(volume);
	}
	else if (i == 3)
	{
		this->soundChannel[3]->setVolume(volume);
		this->soundChannel[4]->setVolume(volume);
		this->soundChannel[5]->setVolume(volume);
		this->soundChannel[6]->setVolume(volume);
		this->soundChannel[7]->setVolume(volume);
		this->soundChannel[8]->setVolume(volume);
		this->soundChannel[9]->setVolume(volume);
	}
	else if (i == 4)
	{
		this->soundChannel[10]->setVolume(volume);
	}
	else if (i == 5)
	{
		this->soundChannel[11]->setVolume(volume);
	}
	else if (i == 6)
	{
		this->soundChannel[12]->setVolume(volume);
	}
	else if (i == 7)
	{
		this->soundChannel[13]->setVolume(volume);
	}
	else if (i == 8)
	{
		this->soundChannel[14]->setVolume(volume);
	}
	else if (i == 9)
	{
		this->soundChannel[15]->setVolume(volume);
	}
	else if (i == 10)
	{
		this->soundChannel[16]->setVolume(volume);
	}
}
void SoundManager::stopSound(int i)
{
	if (i == 0)
	{
		this->soundChannel[0]->stop();
	}
	else if (i == 1)
	{
		this->soundChannel[1]->stop();
	}
	else if (i == 2)
	{
		this->soundChannel[2]->stop();
	}
	else if (i == 3)
	{
		this->soundChannel[3]->stop();
		this->soundChannel[4]->stop();
		this->soundChannel[5]->stop();
		this->soundChannel[6]->stop();
		this->soundChannel[7]->stop();
		this->soundChannel[8]->stop();
		this->soundChannel[9]->stop();
	}
	else if (i == 4)
	{
		this->soundChannel[10]->stop();
	}
	else if (i == 5)
	{
		this->soundChannel[11]->stop();
	}
	else if (i == 6)
	{
		this->soundChannel[12]->stop();
	}
	else if (i == 7)
	{
		this->soundChannel[13]->stop();
	}
	else if (i == 8)
	{
		this->soundChannel[14]->stop();
	}
	else if (i == 9)
	{
		this->soundChannel[15]->stop();
	}
	else if (i == 10)
	{
		this->soundChannel[16]->stop();
	}
}
void SoundManager::togglePauseSound(int i, bool state)
{
	if (i == 0)
	{
		this->soundChannel[0]->setPaused(state);
	}
	else if (i == 1)
	{
		this->soundChannel[1]->setPaused(state);
	}
	else if (i == 2)
	{
		this->soundChannel[2]->setPaused(state);
	}
	else if (i == 3)
	{
		this->soundChannel[3]->setPaused(state);
		this->soundChannel[4]->setPaused(state);
		this->soundChannel[5]->setPaused(state);
		this->soundChannel[6]->setPaused(state);
		this->soundChannel[7]->setPaused(state);
		this->soundChannel[8]->setPaused(state);
		this->soundChannel[9]->setPaused(state);
	}
	else if (i == 4)
	{
		this->soundChannel[10]->setPaused(state);
	}
	else if (i == 5)
	{
		this->soundChannel[11]->setPaused(state);
	}
	else if (i == 6)
	{
		this->soundChannel[12]->setPaused(state);
	}
	else if (i == 7)
	{
		this->soundChannel[13]->setPaused(state);
	}
	else if (i == 8)
	{
		this->soundChannel[14]->setPaused(state);
	}
	else if (i == 9)
	{
		this->soundChannel[15]->setPaused(state);
	}
	else if (i == 10)
	{
		this->soundChannel[16]->setPaused(state);
	}
}

void SoundManager::playAllAmbient()
{
	//Theme
	this->system->playSound(this->soundList[0], 0, false, &this->soundChannel[0]);

	////Crow
	//this->system->playSound(this->soundList[1], 0, false, &this->soundChannel[1]);

	////Cave
	this->system->playSound(this->soundList[2], 0, false, &this->soundChannel[2]);

	////Water
	//this->system->playSound(this->soundList[3], 0, false, &this->soundChannel[3]);
	//this->system->playSound(this->soundList[3], 0, false, &this->soundChannel[4]);
	//this->system->playSound(this->soundList[3], 0, false, &this->soundChannel[5]);
	//this->system->playSound(this->soundList[3], 0, false, &this->soundChannel[6]);
	//this->system->playSound(this->soundList[3], 0, false, &this->soundChannel[7]);
	//this->system->playSound(this->soundList[3], 0, false, &this->soundChannel[8]);
	//this->system->playSound(this->soundList[3], 0, false, &this->soundChannel[9]);

	////Easter
	//this->system->playSound(this->soundList[4], 0, false, &this->soundChannel[10]);

	//Native
	this->system->playSound(this->soundList[5], 0, false, &this->soundChannel[11]);

	////Cheering
	this->system->playSound(this->soundList[8], 0, false, &this->soundChannel[14]);

	//FMOD_VECTOR tempPos = { 1960.82f, 0.0f, 19.0f };
	//FMOD_VECTOR tempVel = { 0.1f, 0.1f, 0.1f };
	//this->soundChannel[3]->set3DAttributes(&tempPos, &tempVel, 0);

	//Volume
	this->setVolume(0, 0.2f);
	//this->setVolume(1, 0.4f);
	this->setVolume(2, 0.4f);
	//this->setVolume(3, 0.1f);
	//this->setVolume(4, 0.4f);
	this->setVolume(5, 0.4f);
	this->setVolume(8, 0.4f);

	this->togglePauseSound(2, true);
	this->togglePauseSound(5, true);
	this->togglePauseSound(8, true);
}
void SoundManager::pauseAllAmbient(bool state)
{
	if (this->faze1 == false && this->faze2 == false && this->enteredCave == false && this->enteredSecret == false)
	{
		this->soundChannel[0]->setPaused(state);
	}
	else if (this->faze1 == false && this->faze2 == false && this->enteredCave == true && this->enteredSecret == false)
	{
		this->soundChannel[2]->setPaused(state);
	}
	else if (this->faze1 == false && this->faze2 == false && this->enteredCave == false && this->enteredSecret == true)
	{
		this->soundChannel[11]->setPaused(state);
	}
	else if (this->faze1 == true)
	{
		this->soundChannel[0]->setPaused(state);
		this->soundChannel[2]->setPaused(state);
	}
	else if (this->faze2 == true)
	{
		this->soundChannel[0]->setPaused(state);
		this->soundChannel[11]->setPaused(state);
	}

	if (this->ending == true)
	{
		this->soundChannel[14]->setPaused(state);
	}

	this->soundChannel[10]->setPaused(state);
	this->soundChannel[12]->setPaused(state);
	this->soundChannel[13]->setPaused(state);
	this->soundChannel[15]->setPaused(state);
	this->soundChannel[16]->setPaused(state);

}
void SoundManager::stopAllAmbient()
{
	/*for (int i = 0; i < 16; i++)
	{
		this->soundChannel[i]->stop();
	}*/

	this->soundChannel[0]->stop();
	this->soundChannel[11]->stop();
	this->soundChannel[12]->stop();
	this->soundChannel[13]->stop();
	this->soundChannel[14]->stop();
	this->soundChannel[15]->stop();
	this->soundChannel[16]->stop();
}

void SoundManager::createFMOD()
{
	this->loadSound();
	//this->addReverb(11);
	//this->placeReverbs();
}
void SoundManager::loadSound()
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

	/*
	########################################
	#				  Ambient			   #
	########################################
	*/

	fr = system->createSound(".\\Assets\\Sound\\Theme.mp3", FMOD_LOOP_NORMAL, 0, &this->soundList[0]);
	if (fr != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", fr, FMOD_ErrorString(fr));
		getchar();
		exit(-1);
	}


	fr = system->createSound(".\\Assets\\Sound\\Single Crow.mp3", FMOD_3D_HEADRELATIVE, 0, &this->soundList[1]);
	if (fr != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", fr, FMOD_ErrorString(fr));
		getchar();
		exit(-1);
	}

	fr = system->createSound(".\\Assets\\Sound\\Cave.mp3", FMOD_LOOP_NORMAL, 0, &this->soundList[2]);
	if (fr != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", fr, FMOD_ErrorString(fr));
		getchar();
		exit(-1);
	}

	fr = system->createSound(".\\Assets\\Sound\\Water.mp3", FMOD_3D_HEADRELATIVE, 0, &this->soundList[3]);
	if (fr != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", fr, FMOD_ErrorString(fr));
		getchar();
		exit(-1);
	}


	fr = system->createSound(".\\Assets\\Sound\\Easter.mp3", FMOD_2D , 0, &this->soundList[4]);
	if (fr != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", fr, FMOD_ErrorString(fr));
		getchar();
		exit(-1);
	}

	fr = system->createSound(".\\Assets\\Sound\\Native.mp3", FMOD_LOOP_NORMAL, 0, &this->soundList[5]);
	if (fr != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", fr, FMOD_ErrorString(fr));
		getchar();
		exit(-1);
	}

	/*
	########################################
	#				Character			   #
	########################################
	*/

	fr = system->createSound(".\\Assets\\Sound\\Jump.mp3", FMOD_2D, 0, &this->soundList[6]);
	if (fr != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", fr, FMOD_ErrorString(fr));
		getchar();
		exit(-1);
	}

	fr = system->createSound(".\\Assets\\Sound\\Crumbling.mp3", FMOD_2D, 0, &this->soundList[7]);
	if (fr != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", fr, FMOD_ErrorString(fr));
		getchar();
		exit(-1);
	}

	/*
	########################################
	#				Triggers			   #
	########################################
	*/

	fr = system->createSound(".\\Assets\\Sound\\Cheering.mp3", FMOD_2D, 0, &this->soundList[8]);
	if (fr != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", fr, FMOD_ErrorString(fr));
		getchar();
		exit(-1);
	}

	fr = system->createSound(".\\Assets\\Sound\\Lever.mp3", FMOD_2D, 0, &this->soundList[9]);
	if (fr != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", fr, FMOD_ErrorString(fr));
		getchar();
		exit(-1);
	}

	fr = system->createSound(".\\Assets\\Sound\\Button.mp3", FMOD_2D, 0, &this->soundList[10]);
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
	if (this->amountOfSounds + addedReverb >= this->soundCap)
	{
		this->expandReverb();
	}

	int tempNrReverbs = this->amountOfSounds + addedReverb;

	std::cout << std::endl << std::endl;
	
	for (int i = this->amountOfSounds; i < tempNrReverbs; i++)
	{
		this->system->createReverb3D(&this->Reverb[i]);

		Reverb[i]->setProperties(prop[0]);
		Reverb[i]->getProperties(prop[i+1]);
		std::cout << i+1 << " " << prop[i+1]->DecayTime << std::endl;

		prop[0]->DecayTime += 200;
	}

	this->amountOfSounds += addedReverb;
}
void SoundManager::expandReverb()
{
	FMOD::Reverb3D** tempReverb = new FMOD::Reverb3D*[this->soundCap + 20];
	FMOD_REVERB_PROPERTIES** tempProp = new FMOD_REVERB_PROPERTIES*[this->soundCap + 20];

	for (int i = 0; i < this->amountOfSounds + 1; i++)
	{
		tempReverb[i] = this->Reverb[i];
		tempProp[i] = this->prop[i];
	}

	for (int i = this->amountOfSounds+1; i < this->soundCap + 20; i++)
	{
		tempProp[i] = new FMOD_REVERB_PROPERTIES;
	}
	this->Reverb = tempReverb;
	this->prop = tempProp;

	this->soundCap += 20;
}
void SoundManager::placeReverbs()
{
	/*
	########################################
	#				  Crows				   #
	########################################
	*/
	FMOD_VECTOR tempVec = { 43.6001f, 0.0f, 44.6001f };
	this->Reverb[7]->set3DAttributes(&tempVec, 0, 200.f);
	this->Reverb[7]->setActive(true);

	/*
	########################################
	#				  Cave				   #
	########################################
	*/

	tempVec = { 1171.41f, 0.0f, 47.6001f };
	this->Reverb[8]->set3DAttributes(&tempVec, 0, 200.f);
	this->Reverb[8]->setActive(true);

	/* 
	########################################
	#				  Water				   #
	########################################
	*/
	tempVec = { 111.339f, 0.0f, 42.0001f };
	this->Reverb[0]->set3DAttributes(&tempVec, 0, 200.f);
	this->Reverb[0]->setActive(true);

	tempVec = { 176.798f, 0.0f, 35.6001f };
	this->Reverb[1]->set3DAttributes(&tempVec, 0, 200.f);
	this->Reverb[1]->setActive(true);

	tempVec = { 221.998f, 0.0f, 78.9999f };
	this->Reverb[2]->set3DAttributes(&tempVec, 0, 200.f);
	this->Reverb[2]->setActive(true);

	/*
	########################################
	#				  Swamp				   #
	########################################
	*/
	tempVec = { 1788.06f, 0.0f, 36.6001f };
	this->Reverb[3]->set3DAttributes(&tempVec, 0, 200.f);
	this->Reverb[3]->setActive(true);

	tempVec = { 1762.46f, 0.0f, 142.199f };
	this->Reverb[4]->set3DAttributes(&tempVec, 0, 200.f);
	this->Reverb[4]->setActive(true);

	tempVec = { 1630.9f, 0.0f, 140.599f };
	this->Reverb[5]->set3DAttributes(&tempVec, 0, 200.f);
	this->Reverb[5]->setActive(true);

	tempVec = { 1633.3f, 0.0f, 40.4001f };
	this->Reverb[6]->set3DAttributes(&tempVec, 0, 200.f);
	this->Reverb[6]->setActive(true);

	/*
	########################################
	#				 Easter			       #
	########################################
	*/

	tempVec = { 1284.18f, 0.0f, 59.4002f };
	this->Reverb[9]->set3DAttributes(&tempVec, 0, 200.f);
	this->Reverb[9]->setActive(true);

	/*
	########################################
	#				Cheering			   #
	########################################
	*/

	tempVec = { 1960.82f, 0.0f, 19.0f };
	this->Reverb[10]->set3DAttributes(&tempVec, 0, 200.f);
	this->Reverb[10]->setActive(true);

}

void SoundManager::setListnerPos(FMOD_VECTOR pos)
{
	this->system->set3DListenerAttributes(0, &pos, 0, 0, 0);
}


void SoundManager::checkUnique(float posX, float posZ)
{
	this->checkEnteringCave(posX, posZ);
	this->checkExitingCave(posX, posZ);
	this->checkEnding(posX, posZ);
	this->checkIndian(posX, posZ);
	this->checkBear(posX, posZ);
}
void SoundManager::checkEnteringCave(float posX, float posZ)
{
	if (posX >= 1049 && posX <= 1050 && posZ >= 8 && posZ <= 45)
	{
		this->enteredCave = false;
		this->faze1 = false;
		this->togglePauseSound(2, true);
		this->togglePauseSound(0, false);
		this->setVolume(0, 0.2f);
	}
	if (posX >= 1054 && posX <= 1055 && posZ >= 8 && posZ <= 45)
	{
		this->faze1 = true;
		this->setVolume(0, 0.18f);
		this->setVolume(2, 0.001f);
	}
	if (posX >= 1059 && posX <= 1060 && posZ >= 8 && posZ <= 45)
	{
		this->setVolume(0, 0.15f);
		this->setVolume(2, 0.05f);
	}
	if (posX >= 1069 && posX <= 1070 && posZ >= 8 && posZ <= 45)
	{
		this->setVolume(0, 0.10f);
		this->setVolume(2, 0.15f);
	}
	if (posX >= 1079 && posX <= 1080 && posZ >= 8 && posZ <= 45)
	{
		this->setVolume(0, 0.05f);
		this->setVolume(2, 0.25f);
	}
	if (posX >= 1084 && posX <= 1085 && posZ >= 8 && posZ <= 50)
	{
		this->faze1 = true;
		this->setVolume(0, 0.01f);
		this->setVolume(2, 0.2999f);
	}
	if (posX >= 1099 && posX <= 1100 && posZ >= 8 && posZ <= 60)
	{
		this->enteredCave = true;
		this->faze1 = false;
		this->togglePauseSound(0, true);
		this->togglePauseSound(2, false);
		this->setVolume(2, 0.3f);
	}
}
void SoundManager::checkExitingCave(float posX, float posZ)
{
	if (posX >= 1220 && posX <= 1221 && posZ >= -10 && posZ <= 70)
	{
		this->faze1 = false;
		this->enteredCave = true;
		this->togglePauseSound(0, true);
		this->togglePauseSound(2, false);
		this->setVolume(2, 0.3f);
	}
	if (posX >= 1225 && posX <= 1226 && posZ >= -10 && posZ <= 70)
	{
		this->faze1 = true;
		this->setVolume(0, 0.01f);
		this->setVolume(2, 0.2999f);
	}
	if (posX >= 1230 && posX <= 1231 && posZ >= -10 && posZ <= 70)
	{
		this->setVolume(0, 0.05f);
		this->setVolume(2, 0.25f);
	}
	if (posX >= 1240 && posX <= 1241 && posZ >= -10 && posZ <= 70)
	{
		this->setVolume(0, 0.10f);
		this->setVolume(2, 0.15f);
	}
	if (posX >= 1250 && posX <= 1251 && posZ >= -10 && posZ <= 70)
	{
		this->setVolume(0, 0.15f);
		this->setVolume(2, 0.05f);
	}
	if (posX >= 1255 && posX <= 1256 && posZ >= -10 && posZ <= 70)
	{
		this->faze1 = true;
		this->setVolume(0, 0.18f);
		this->setVolume(2, 0.001f);
	}
	if (posX >= 1260 && posX <= 1261 && posZ >= -10 && posZ <= 70)
	{
		this->faze1 = false;
		this->enteredCave = false;
		this->togglePauseSound(2, true);
		this->togglePauseSound(0, false);
		this->setVolume(0, 0.2f);
	}
}
void SoundManager::checkEnding(float posX, float posZ)
{
	if (posX >= 1910 && posX <= 1911 && posZ >= -5 && posZ <= 60)
	{
		this->togglePauseSound(8, false);
		this->setVolume(8, 0.3f);
		this->ending = true;
	}
}
void SoundManager::checkIndian(float posX, float posZ)
{
	if (posX >= 987 && posX <= 1036 && posZ >= 59 && posZ <= 60)
	{
		this->faze2 = false;
		this->enteredSecret = false;
		this->togglePauseSound(5, true);
		this->togglePauseSound(0, false);
		this->setVolume(0, 0.2f);
	}
	if (posX >= 987 && posX <= 1036 && posZ >= 64 && posZ <= 65)
	{
		this->faze2 = true;
		this->setVolume(0, 0.18f);
		this->setVolume(5, 0.001f);
	}
	if (posX >= 987 && posX <= 1036 && posZ >= 69 && posZ <= 70)
	{
		this->setVolume(0, 0.15f);
		this->setVolume(5, 0.05f);
	}
	if (posX >= 987 && posX <= 1036 && posZ >= 79 && posZ <= 80)
	{
		this->setVolume(0, 0.10f);
		this->setVolume(5, 0.15f);
	}
	if (posX >= 987 && posX <= 1036 && posZ >= 89 && posZ <= 90)
	{
		this->setVolume(0, 0.05f);
		this->setVolume(5, 0.25f);
	}
	if (posX >= 987 && posX <= 1036 && posZ >= 94 && posZ <= 95)
	{
		this->faze2 = true;
		this->setVolume(0, 0.01f);
		this->setVolume(5, 0.2999f);
	}
	if (posX >= 987 && posX <= 1036 && posZ >= 99 && posZ <= 100)
	{
		this->faze2 = false;
		this->enteredSecret = true;
		this->togglePauseSound(0, true);
		this->togglePauseSound(5, false);
		this->setVolume(5, 0.3f);
	}
}
void SoundManager::checkBear(float posX, float posZ)
{
	this->tButtonPress = GetCurrentTime();
	if (posX >= 1270 && posX <= 1272 && posZ <= 53 && posZ >= 52)
	{
		if (this->tButtonPress - this->lTimePress >= 25000)
		{
			if (GetAsyncKeyState(0x45)) //e
			{
				this->playSound(4);
				lTimePress = GetCurrentTime();
			}
		}
	}
}
