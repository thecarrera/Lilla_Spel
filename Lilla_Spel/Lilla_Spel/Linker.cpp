#include "Linker.h"

Linker::Linker()
{
}
Linker::~Linker()
{
}
void Linker::Clean()
{
}

void Linker::LoadModel(std::string fileName, ID3D11Device* gDevice, ID3D11Buffer* &gVertexBuffer, ID3D11Buffer* &shaderBuffer)
{
	this->obj.LoadModel(fileName, gDevice, gVertexBuffer, shaderBuffer);
}

void Linker::Texture(ID3D11Device* &gDevice, ID3D11DeviceContext* &gDeviceContext, ID3D11ShaderResourceView* &gTextureRTV)
{
	if (strcmp(obj.returnDiffuseName(), "None"))
	{
		this->obj.Texture(gDevice, gDeviceContext, gTextureRTV);
	}
}

int Linker::ReturnVertexInfo()
{
	return obj.ReturnVertexInfo();
}

int Linker::getAmountOfVerticies()
{
	return this->obj.getAmountOfVertecies();
}
