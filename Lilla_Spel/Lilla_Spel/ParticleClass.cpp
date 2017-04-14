#include "ParticleClass.h"

ParticleClass::ParticleClass(ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext)
{
	this->gDevice = gDevice;
	this->gDeviceContext = gDeviceContext;
}

ParticleClass::ParticleClass(ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext, const ParticleClass& particleClass)
{
	this->gDevice = gDevice;
	this->gDeviceContext = gDeviceContext;
}

ParticleClass::~ParticleClass()
{

}

bool ParticleClass::initialize(WCHAR* textureFileName)
{
	bool result = true;

	//texture för partiklar 
	result = loadTexture(this->gDevice, textureFileName);
	if (!result)
	{
		return false;
		//filen fanns inte
	}

	//partiklesystem initialize
	result = initializeParticleSystem();
	if (!result)
	{
		return false;
		//partikelsystemet
	}

	//partikel buffer
	result = initializeBuffers(this->device);
	if (!result)
	{
		return false;
	}

	//lyckades med allt
	return true;
}

void ParticleClass::kill()
{
	//bort med buffer, är tydligen en sak?
	shutdownBuffers();

	//bort med partikelsystemet
	shutdownParticleSystem();

	//bort med texturen
	releaseTexture();
}

bool ParticleClass::frameUpdate(float frameTime)
{
	bool result = true;

	//bort med gamla partiklar
	killParticles();

	//nya partiklar
	emitParticles(frameTime);

	//updatera positionen av partiklarna
	updateParticles(frameTime);

	//måste uppdatera buffern om uppdaterar positionen
	result = updateBuffers(this->gDeviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

void ParticleClass::render()
{
	//vertex & index buffern ska uppdateras, vi gör det härifrån
	renderBuffers(this->gDeviceContext);
}

void ParticleClass::createTexture()
{
	//HRESULT hr = DirectX::CreateWICTextureFromFile(this->gDevice, this->gDeviceContext, L"../resource/Maps/nånting.png", &this->textureResoure, &this->textureView);
	HRESULT hr = DirectX::
	if (FAILED(hr))
	{
		MessageBox(0, "texture creation failed", "error", MB_OK);
	}
}







int ParticleClass::getmIndexCount()
{
	return this->mIndexCount
}