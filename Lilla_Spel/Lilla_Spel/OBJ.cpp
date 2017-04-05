#include "Importers.h"

/*
#################################################################################################################################
#																																#
#							All the components for the basic OBJ import. 														#
#							Note: This at the moment only takes one texture and applies it the whole model.						#
#							This is a problem im aware of and im not considering to fixing due to time and planning.			#
#																																#
#################################################################################################################################
*/

//OBJ
OBJ::OBJ()
{
}
OBJ::~OBJ()
{
	this->Clean();
}
void OBJ::Clean()
{

}

void OBJ::setGroupName(char* cmd)
{
	for (int i = 0; i < 256; i++)
	{
		this->groupName[i] = cmd[i];
	}
}

void OBJ::addVertexPos(DirectX::XMFLOAT3 inputValues)
{
	this->vertexPos.push_back(inputValues);
	this->amountOfVertecies++;
}
void OBJ::addVertexNorm(DirectX::XMFLOAT3 inputValues)
{
	this->vertexNorm.push_back(inputValues);
}
void OBJ::addVertexUV(DirectX::XMFLOAT2 inputValues)
{
	this->vertexTex.push_back(inputValues);
}

DirectX::XMFLOAT3 OBJ::getVertPosAt(int index)
{
	return this->vertexPos.at(index);
}
DirectX::XMFLOAT3 OBJ::getVertNormAt(int index)
{
	return this->vertexNorm.at(index);
}
DirectX::XMFLOAT2 OBJ::getVertUVAt(int index)
{
	return this->vertexTex.at(index);
}

void OBJ::createFaces(DirectX::XMINT3 temp_vert, int count)
{
	TriangleInfo triangleArray;

	DirectX::XMFLOAT3 inputValues;
	DirectX::XMFLOAT2 inputTex;

	inputValues = this->getVertPosAt(temp_vert.x);

	triangleArray.x = inputValues.x;
	triangleArray.y = inputValues.y;
	triangleArray.z = inputValues.z;

	inputValues = this->getVertNormAt(temp_vert.y);

	triangleArray.x = inputValues.x;
	triangleArray.y = inputValues.y;
	triangleArray.z = inputValues.z;

	inputValues = this->getVertPosAt(temp_vert.z);

	triangleArray.u = inputValues.x;
	triangleArray.v = inputValues.y;

	this->vertexFace.push_back(triangleArray);
}

UINT32 OBJ::returnVertexInfo()
{
	return sizeof(TriangleInfo);
}
auto OBJ::returnVertexData()
{
	return this->vertexFace.data();
}
int OBJ::getAmountOfVertecies()
{
	return this->amountOfVertecies;
}

//Textures
Textures::Textures()
{
	this->objTex.Kd.x = 1;
	this->objTex.Kd.y = 1;
	this->objTex.Kd.z = 1;

	this->objTex.Ka.x = 1;
	this->objTex.Ka.y = 1;
	this->objTex.Ka.z = 1;

	this->objTex.Ks.x = 1;
	this->objTex.Ks.y = 1;
	this->objTex.Ks.z = 1;

	this->objTex.garbagex = 0;
	this->objTex.garbagey = 0;
	this->objTex.garbagez = 0;

}
Textures::~Textures() 
{

}
void Textures::Clean()
{
}

void Textures::setDiffValues(DirectX::XMFLOAT3 inputValues)
{
	this->objTex.Kd.x = inputValues.x;
	this->objTex.Kd.y = inputValues.y;
	this->objTex.Kd.z = inputValues.z;
}
void Textures::setAmbValues(DirectX::XMFLOAT3 inputValues)
{
	this->objTex.Ka.x = inputValues.x;
	this->objTex.Ka.y = inputValues.y;
	this->objTex.Ka.z = inputValues.z;
}
void Textures::setSpecValues(DirectX::XMFLOAT3 inputValues)
{
	this->objTex.Ks.x = inputValues.x;
	this->objTex.Ks.y = inputValues.y;
	this->objTex.Ks.z = inputValues.z;
}

void Textures::setMatName(char* cmd)
{
	for (int i = 0; i < 256; i++)
	{
		matList.matName[i] = cmd[i];
	}
	std::cout << matList.matName;
}
void Textures::setDiffAmbName(char* cmd)
{
	for (int i = 0; i < 256; i++)
	{
		matList.daName[i] = cmd[i];
	}
}

char* Textures::returnDiffuseName()
{
	return this->matList.daName;
}
int Textures::returnTextureInfo()
{
	return sizeof(TextureInfo);
}