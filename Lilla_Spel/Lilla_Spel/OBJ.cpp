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

void OBJ::createFaces(DirectX::XMINT3 temp_vert)
{
	DirectX::XMFLOAT3 inputValues;
	DirectX::XMFLOAT2 inputTex;

	inputValues = this->getVertPosAt(temp_vert.x);

	triangleArray.x = inputValues.x;
	triangleArray.y = inputValues.y;
	triangleArray.z = inputValues.z;

	inputValues = this->getVertNormAt(temp_vert.y);

	triangleArray.nx = vertexNorm.at(temp_vert[i].z - 1).x;
	triangleArray.ny = vertexNorm.at(temp_vert[i].z - 1).y;
	triangleArray.nz = vertexNorm.at(temp_vert[i].z - 1).z;

	inputValues = this->getVertPosAt(temp_vert.z);

	triangleArray.u = vertexTex.at(temp_vert[i].y - 1).x;
	triangleArray.v = vertexTex.at(temp_vert[i].y - 1).y;

	this->vertexFace.push_back(triangleArray);
}

int OBJ::returnVertexInfo()
{
	return sizeof(TriangleInfo);
}
int OBJ::getAmountOfVertecies()
{
	return this->amountOfVertecies;
}


//Textures
Textures::Textures()
{

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

const char* Textures::returnDiffuseName()
{
	return this->matList.daName;
}
