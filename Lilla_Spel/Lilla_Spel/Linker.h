#pragma once

#include "ObjImport.h" 

class Linker
{
public:
	Linker();
	~Linker();
	void Clean();

	void LoadModel(std::string fileName, ID3D11Device* gDevice, ID3D11Buffer* &gVertexBuffer, ID3D11Buffer* &shaderBuffer);
	void Texture(ID3D11Device* &gDevice, ID3D11DeviceContext* &gDeviceContext, ID3D11ShaderResourceView* &gTextureRTV);
	int ReturnVertexInfo();
	int getAmountOfVerticies();

private:
	OBJ obj;

	std::vector<ID3D11Buffer*> shaderBuffers;
};
