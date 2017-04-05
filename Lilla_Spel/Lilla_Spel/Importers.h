#pragma once

#include "Includes.h"

/*
#################################################################################################################################
#																																#
#										All the components for our importers.													#
#																																#
#################################################################################################################################
*/
class OBJ;
class Textures;

class OBJHandler
{
public:
	OBJHandler();
	~OBJHandler();
	void Clean();

	void LoadModel(std::string fileName, ID3D11Device* gDevice, ID3D11Buffer* &gVertexBuffer, ID3D11Buffer* &shaderBuffer);
	void Texture(ID3D11Device* &gDevice, ID3D11DeviceContext* &gDeviceContext, ID3D11ShaderResourceView* &gTextureRTV);

	int findAmountOfMeshes(std::string fileName);
	int findAmountOfTextures(std::string fileName);

	int getAmountOfVertecies();
	UINT32 returnVertexInfo();
	const char* returnDiffuseName();

	DirectX::XMMATRIX worldM;

private:
	std::vector<OBJ> mesh;
	std::vector<Textures> meshTex;

	int objCap;
	int amountOfTextures, amountOfMeshes;
};

class OBJ
{
	struct VertexInfo;
	struct TriangleInfo;
public:
	OBJ();
	~OBJ();
	void Clean();

	void setGroupName(char* cmd);

	void addVertexPos(DirectX::XMFLOAT3 inputValues);
	void addVertexNorm(DirectX::XMFLOAT3 inputValues);
	void addVertexUV(DirectX::XMFLOAT2 inputValues);

	DirectX::XMFLOAT3 getVertPosAt(int index);
	DirectX::XMFLOAT3 getVertNormAt(int index);
	DirectX::XMFLOAT2 getVertUVAt(int index);

	void createFaces(DirectX::XMINT3 temp_vert, int);

	UINT32 returnVertexInfo();
	auto returnVertexData();
	int getAmountOfVertecies();
private:
	struct TriangleInfo
	{
		float x, y, z;
		float nx, ny, nz;
		float u, v;
	};

	std::vector<DirectX::XMFLOAT3> vertexPos;
	std::vector<DirectX::XMFLOAT3> vertexNorm;
	std::vector<DirectX::XMFLOAT2> vertexTex;
	std::vector<TriangleInfo> vertexFace;

	char groupName[256];

	int amountOfVertecies;
	int vCapacity, tCapacity, nCapacity, fCapacity;
};

class Textures
{
public:
	Textures();
	~Textures();
	void Clean();

	void setDiffValues(DirectX::XMFLOAT3 inputValues);
	void setAmbValues(DirectX::XMFLOAT3 inputValues);
	void setSpecValues(DirectX::XMFLOAT3 inputValues);

	void setMatName(char* cmd);
	void setDiffAmbName(char* cmd);
	void setSpecName(char* cmd);
	void setNormName(char* cmd);

	char* returnDiffuseName();
	int returnTextureInfo();

private:
	struct TextureInfo
	{
		DirectX::XMFLOAT3 Kd;
		DirectX::XMFLOAT3 Ka;
		DirectX::XMFLOAT3 Ks;
		float garbagex, garbagey, garbagez;
	};
	struct MatList
	{
		char matName[256] = { "None" };
		char daName[256] = { "None" };
		char spName[256] = { "None" };
		char bumpName[256] = { "None" };
	};

	TextureInfo objTex;
	MatList matList;
};