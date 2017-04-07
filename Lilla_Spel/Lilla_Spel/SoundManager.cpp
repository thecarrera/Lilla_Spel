#include "SoundManager.h"

SoundManager::SoundManager()
{
	DirectS = 0;
	PrimaryBuffer = 0;
	SecondaryBuffer = 0;
	AlternativeBuffer = 0;

}

SoundManager::~SoundManager()
{
}
bool SoundManager::initialize(HWND* hwnd)
{
	bool hres;

	hres = initializeDS(hwnd);

	return false;
}
void SoundManager::shutdown()
{
	shutdownWave(&SecondaryBuffer);

	shutdownDS();

}
void SoundManager::Play(char * filepath, bool shouldLoop)
{
	loadWave(filepath, &SecondaryBuffer);
	playWave();
	


}
bool SoundManager::initializeDS(HWND* hwnd)
{
	HRESULT hres;
	DSBUFFERDESC bufferDsc;
	WAVEFORMATEX waveForm;

	//Create and set cooperative level
	hres = DirectSoundCreate8(NULL, &DirectS, NULL);
	if (FAILED(hres)) {
		return false;
	}
	hres = DirectS->SetCooperativeLevel(*hwnd, DSSCL_PRIORITY);
	if (FAILED(hres)) {
		return false;
	}
	//set up the Primarybuffer, as well as flag it as the controller for our Volume
	bufferDsc.dwSize = sizeof(DSBUFFERDESC);
	bufferDsc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	bufferDsc.dwBufferBytes = 0;
	bufferDsc.dwReserved = 0;
	bufferDsc.lpwfxFormat = NULL;
	bufferDsc.guid3DAlgorithm = DS3DALG_NO_VIRTUALIZATION;

	hres = DirectS->CreateSoundBuffer(&bufferDsc, &PrimaryBuffer, NULL);
	if (FAILED(hres)) {
		return false;
	}
	//we want high quality Sound, so we set up the Wave format to handle it(CHANGE IF WE CAN'T SUPPORT)
	waveForm.wFormatTag = WAVE_FORMAT_PCM;
	waveForm.nSamplesPerSec = 44100;
	waveForm.wBitsPerSample = 16;
	waveForm.nChannels = 2;
	waveForm.nBlockAlign = (waveForm.wBitsPerSample / 8)*waveForm.nChannels;
	waveForm.nAvgBytesPerSec = waveForm.nSamplesPerSec * waveForm.nBlockAlign;
	waveForm.cbSize = 0;

	if (FAILED(hres)) {
		return false;
	}


	return true;
}

void SoundManager::shutdownDS()
{
	if (PrimaryBuffer) {
		PrimaryBuffer->Release();
		PrimaryBuffer = 0;
	}

	if (DirectS) {
		DirectS->Release();
		DirectS = 0;
	}
}

bool SoundManager::loadWave(char * filepath, IDirectSoundBuffer8 ** SecondaryBuffer)
{
	int error;
	FILE* filePtr;
	unsigned int count;
	Wave waveHeader;
	WAVEFORMATEX waveFormat;
	DSBUFFERDESC bufferdsc;
	HRESULT hres;
	IDirectSoundBuffer* tmpBuff;
	unsigned char* waveData;
	unsigned char* bufferptr;
	unsigned long bufferSz;

	error = fopen_s(&filePtr, filepath, "rb");
	if (error != 0) {
		return false;
	}

	count = fread(&waveHeader, sizeof(waveHeader), 1, filePtr);
	if (count != 1)
	{
		return false;
	}
	if ((waveHeader.chunkID[0] != 'R') || (waveHeader.chunkID[1] != 'I') ||
		(waveHeader.chunkID[2] != 'F') || (waveHeader.chunkID[3] != 'F')) {
		return false;
	}
	if ((waveHeader.format[0] != 'W') || (waveHeader.format[1] != 'A') ||
		(waveHeader.format[2] != 'V') || (waveHeader.format[3] != 'E')) {
		return false;
	}
	if ((waveHeader.subChunkId[0] != 'f') || (waveHeader.subChunkId[1] != 'm') ||
		(waveHeader.subChunkId[2] != 't') || (waveHeader.subChunkId[3] != ' ')) {
		return false;
	}
	if (waveHeader.audioFormat != WAVE_FORMAT_PCM) {
		return false;
	}
	if (waveHeader.numChannels != 2)
	{
		return false;
	}
	if(waveHeader.sampleRate != 44100)
	{
		return false;
	}
	if (waveHeader.bitPerSecond != 16) {
		return false;
	}
	if ((waveHeader.dataChuckId[0] != 'd') || (waveHeader.dataChuckId[1] != 'a') ||
		(waveHeader.dataChuckId[2] != 't') || (waveHeader.dataChuckId[3] != 'a')) {
		return false;
	}

	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8)*waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec*waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	bufferdsc.dwSize = sizeof(DSBUFFERDESC);
	bufferdsc.dwFlags = DSBCAPS_CTRLVOLUME;
	bufferdsc.dwBufferBytes = waveHeader.dataSize;
	bufferdsc.dwReserved = 0;
	bufferdsc.lpwfxFormat = &waveFormat;
	bufferdsc.guid3DAlgorithm = DS3DALG_NO_VIRTUALIZATION;

	hres = DirectS->CreateSoundBuffer(&bufferdsc, &tmpBuff, NULL);
	if (FAILED(hres)) {
		return false;
	}
	hres = tmpBuff->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*SecondaryBuffer);
	if (FAILED(hres)) {
		return false;
	}

	tmpBuff->Release();
	tmpBuff = 0;


	fseek(filePtr, sizeof(waveHeader), SEEK_SET);

	waveData = new unsigned char[waveHeader.dataSize];
	if (!waveData) {
		return false;
	}

	count = fread(waveData, 1, waveHeader.dataSize, filePtr);
	if (count != waveHeader.dataSize) {
		return false;
	}
	error = fclose(filePtr);
	if (error != 0) {
		return false;
	}

	hres = (*SecondaryBuffer)->Lock(0, waveHeader.dataSize, (void**)&bufferptr, (DWORD*)&bufferSz, NULL, 0, 0);
	if (FAILED(hres)) {
		return false;
	}

	memcpy(bufferptr, waveData, waveHeader.dataSize);

	hres = (*SecondaryBuffer)->Unlock((void*)bufferptr, bufferSz, NULL, 0);
	if (FAILED(hres)) {
		return false;
	}

	delete[]waveData;
	waveData = 0;

	return false;
}

void SoundManager::shutdownWave(IDirectSoundBuffer8** SecondaryBuffer)
{
	if (*SecondaryBuffer) {
		(*SecondaryBuffer)->Release();
		*SecondaryBuffer = 0;
	}


}

bool SoundManager::playWave()
{
	HRESULT hres;

	hres = SecondaryBuffer->SetCurrentPosition(0);
	if (FAILED(hres)) {
		return false;
	}
	hres = SecondaryBuffer->SetVolume(DSBVOLUME_MAX);
	if (FAILED(hres)) {
		return false;
	}
	hres = SecondaryBuffer->Play(0, 0, 0);
	if (FAILED(hres)) {
		return false;
	}

	return true;
}


