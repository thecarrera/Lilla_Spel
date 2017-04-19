#include "ParticleClass.h"

ParticleClass::ParticleClass(ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext)
{
	particles = new Particle[];

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

void ParticleClass::initiateParticles()
{

}

void ParticleClass::setVertexBuffer()
{

	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc)); //zeroar memoriet
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(triangleVertices);

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = triangleVertices;
	gDevice->CreateBuffer(&bufferDesc, &data, &gVertexBuffer);
}







int ParticleClass::getmIndexCount()
{
	return this->mIndexCount;
}