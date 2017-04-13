#ifndef PARTICLECLASS_H
#define PARTICLECLASS_H

#include "Includes.h"

//http://www.rastertek.com/dx11tut39.html //referens

class ParticleClass
{
private:
	ID3D11Device* gDevice;

public:
	ParticleClass(ID3D11Device* gDevice);
	ParticleClass(ID3D11Device* gDevice, const ParticleClass& particleClass);
	~ParticleClass();

	bool initialize(ID3D11Device* gDevice, WCHAR* hmm);
	//kolla vad WCHAR var för nåt
};

#endif // !
