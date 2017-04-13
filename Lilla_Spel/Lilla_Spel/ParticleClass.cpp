#include "ParticleClass.h"

ParticleClass::ParticleClass(ID3D11Device* gDevice)
{
	this->gDevice = gDevice;
}

ParticleClass::ParticleClass(ID3D11Device* gDevice, const ParticleClass& particleClass)
{
	this->gDevice = gDevice;
}

ParticleClass::~ParticleClass()
{

}
