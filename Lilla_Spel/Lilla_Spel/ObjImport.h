#pragma once

#include "Includes.h"

class OBJ
{
	struct VertexInfo;
	struct TriangleInfo;
public:
	OBJ();
	~OBJ();
	void Clean();

	void LoadModel(std::string fileName, ID3D11Device* gDevice, ID3D11Buffer* &gVertexBuffer, ID3D11Buffer* &shaderBuffer);
	void Texture(ID3D11Device* &gDevice, ID3D11DeviceContext* &gDeviceContext, ID3D11ShaderResourceView* &gTextureRTV);
	int ReturnVertexInfo();
	const char* returnDiffuseName();

	int getAmountOfVertecies();

private:
	struct TriangleInfo
	{
		float x, y, z;
		float nx, ny, nz;
		float u, v;
	};
	struct TextureInfo
	{
		DirectX::XMFLOAT3 Kd;
		DirectX::XMFLOAT3 Ka;
		DirectX::XMFLOAT3 Ks;
		float garbagex, garbagey, garbagez;
	};
	struct MatList
	{
		char daName[256] = { "None" };
		char spName[256] = { "None" };
		char bumpName[256] = { "None" };
		char matName[256] = { "None" };
	};

	std::vector<DirectX::XMFLOAT3> vertexPos;
	std::vector<DirectX::XMFLOAT2> vertexTex;
	std::vector<DirectX::XMFLOAT3> vertexNorm;
	std::vector<TriangleInfo> vertexFace;
	std::vector<TextureInfo> objTex;
	std::vector<MatList> matList;

	int amountOfVertecies, amountOfTextures;
	int vCapacity, tCapacity, nCapacity, fCapacity;
};