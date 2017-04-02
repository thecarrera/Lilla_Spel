#pragma once

#include "Importers.h" 

/*
#################################################################################################################################
#																																#
#							A handler connecting all of of implemented techniques and modules.									#
#							Making it easier to controll and access functions from the core engine.								#
#							Allowing us to apply funtions and such in form of modules											#
#																																#
#################################################################################################################################
*/

class Linker
{
public:
	Linker();
	~Linker();
	void Clean();

	//OBJ 
	void LoadModel(std::string fileName, ID3D11Device* gDevice, ID3D11Buffer* &gVertexBuffer, ID3D11Buffer* &shaderBuffer);
	void Texture(ID3D11Device* &gDevice, ID3D11DeviceContext* &gDeviceContext, ID3D11ShaderResourceView* &gTextureRTV);
	int ReturnVertexInfo();
	int getAmountOfVerticies();

private:
	OBJ obj;

	std::vector<ID3D11Buffer*> shaderBuffers;
};
