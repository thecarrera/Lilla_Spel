#pragma once
#include <dsound.h>
#include <mmsystem.h>
#include <Windows.h>
#include <stdio.h>
#pragma comment (lib,"dsound.lib")
#pragma comment (lib,"dxguid.lib")
#pragma comment (lib,"winmm.lib")

////////////////////////
// DirectSound (Also known as DirectAudio)
// For Game Engine
///////////////////////
class SoundManager
{
	//Structs and Functions
private:
	//Wave struct, in load, we will validate the data before loading it in to the struct and then passing it to the secondary buffer.
	struct Wave {
		char chunkID[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytePerSecond;
		unsigned short blockAlign;
		unsigned short bitPerSecond;
		char dataChuckId[4];
		unsigned long dataSize;
};
	bool initializeDS(HWND* hwnd);
	void shutdownDS();
	bool loadWave(char* filepath, IDirectSoundBuffer8** SecondaryBuffer);
	void shutdownWave(IDirectSoundBuffer8** SecondaryBuffer);
	bool playWave();

public:
	SoundManager();
	~SoundManager();
	bool initialize(HWND* hwnd);
	void shutdown();
	void Play(char* filepath, bool shouldLoop);

private:
	IDirectSound8* DirectS;
	IDirectSoundBuffer* PrimaryBuffer;
	IDirectSoundBuffer8* SecondaryBuffer;
	IDirectSoundBuffer8* AlternativeBuffer;

};