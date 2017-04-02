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

OBJ::OBJ()
{
	this->amountOfVertecies = 0;
}
OBJ::~OBJ()
{
	this->Clean();
}
void OBJ::Clean()
{

}

void OBJ::LoadModel(std::string fileName, ID3D11Device* gDevice, ID3D11Buffer* &gVertexBuffer, ID3D11Buffer* &shaderBuffer)
{
	std::ifstream ss(fileName, std::ifstream::in);

	char cmd[256] = { 0 };
	char mtlName[256] = { 0 };

	DirectX::XMFLOAT3 inputValues;
	DirectX::XMFLOAT3 temp_vert[3];
	DirectX::XMFLOAT2 inputTex;

	TriangleInfo triangleArray;
	TextureInfo textureArray;
	MatList matL;

	int tempV = 0;
	int tempT = 0;
	int tempN = 0;
	int exist = 0;
	char trash;

	while (ss.good())
	{
		ss >> cmd;
		if (!ss)
		{
			break;
		}
		else if (strcmp(cmd, "mtllib") == 0)
		{
			ss >> mtlName;

			std::ifstream mtl(mtlName, std::ifstream::in);

			while (mtl.good())
			{
				mtl >> cmd;

				if (!mtl)
				{
					break;
				}
				else if (strcmp(cmd, "newmtl") == 0)
				{
					mtl >> cmd;

					for (int i = 0; i < 256; i++)
					{
						matL.matName[i] = cmd[i];
					}
					std::cout << matL.matName;

					mtl >> cmd;

					while (mtl.good() && exist == 0)
					{
						if (strcmp(cmd, "Kd") == 0)
						{
							mtl >> inputValues.x >> inputValues.y >> inputValues.z;

							textureArray.Kd.x = inputValues.x;
							textureArray.Kd.y = inputValues.y;
							textureArray.Kd.z = inputValues.z;

							std::cout << textureArray.Kd.x << " " << textureArray.Kd.y << " " << textureArray.Kd.z;
						}
						else if (strcmp(cmd, "Ka") == 0)
						{
							mtl >> inputValues.x >> inputValues.y >> inputValues.z;

							textureArray.Ka.x = inputValues.x;
							textureArray.Ka.y = inputValues.y;
							textureArray.Ka.z = inputValues.z;
						}
						else if (strcmp(cmd, "Ks") == 0)
						{
							mtl >> inputValues.x >> inputValues.y >> inputValues.z;

							textureArray.Ks.x = inputValues.x;
							textureArray.Ks.y = inputValues.y;
							textureArray.Ks.z = inputValues.z;
						}
						else if (strcmp(cmd, "map_Kd") == 0 || strcmp(cmd, "map_Ka") == 0)
						{
							mtl >> matL.daName;
						}
						//else if (strcmp(cmd, "map_Ks"))
						//{
						//	mtl >> matL.spName;
						//}
						//else if (strcmp(cmd, "map_bump") || strcmp(cmd, "bump"))
						//{
						//	mtl >> matL.bumpName;
						//}
						else if (strcmp(cmd, "newmtl") == 0)
						{
							exist = 1;
						}


						mtl >> cmd;
					}
					this->amountOfTextures++;
				}
				if (exist == 1)
				{
					break;
				}
			}

		}
		else if (strcmp(cmd, "v") == 0)
		{
			ss >> inputValues.x >> inputValues.y >> inputValues.z;
			inputValues.z = (inputValues.z * -1.0f);

			this->vertexPos.push_back(inputValues);

			tempV++;
		}
		else if (strcmp(cmd, "vt") == 0)
		{
			ss >> inputTex.x >> inputTex.y;

			this->vertexTex.push_back(inputTex);

			tempT++;
		}
		else if (strcmp(cmd, "vn") == 0)
		{
			ss >> inputValues.x >> inputValues.y >> inputValues.z;
			inputValues.z = (inputValues.z * -1.0f);

			this->vertexNorm.push_back(inputValues);

			tempN++;
		}
		else if (strcmp(cmd, "f") == 0)
		{
			for (int i = 3; i > 0; i--)
			{
				ss >> temp_vert[i - 1].x >> trash >> temp_vert[i - 1].y >> trash >> temp_vert[i - 1].z;
			}

			for (int i = 0; i < 3; i++)
			{
				triangleArray.x = vertexPos.at(temp_vert[i].x - 1).x;
				triangleArray.y = vertexPos.at(temp_vert[i].x - 1.f).y;
				triangleArray.z = vertexPos.at(temp_vert[i].x - 1).z;

				triangleArray.nx = vertexNorm.at(temp_vert[i].z - 1).x;
				triangleArray.ny = vertexNorm.at(temp_vert[i].z - 1).y;
				triangleArray.nz = vertexNorm.at(temp_vert[i].z - 1).z;

				triangleArray.u = vertexTex.at(temp_vert[i].y - 1).x;
				triangleArray.v = vertexTex.at(temp_vert[i].y - 1).y;

				this->vertexFace.push_back(triangleArray);

				this->amountOfVertecies++;
			}
		}
	}

	textureArray.Ka.x = 1;
	textureArray.Ks.x = 1;
	textureArray.Kd.x = 1;

	textureArray.garbagex = 0;
	textureArray.garbagey = 0;
	textureArray.garbagez = 0;

	this->objTex.push_back(textureArray);
	this->matList.push_back(matL);

	HRESULT hr;

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(TriangleInfo) * this->amountOfVertecies;

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem = this->vertexFace.data();
	hr = gDevice->CreateBuffer(&bufferDesc, &data, &gVertexBuffer);

	if (FAILED(hr))
	{
		return exit(-1);
	}

	D3D11_BUFFER_DESC cBufferDesc;
	ZeroMemory(&cBufferDesc, sizeof(cBufferDesc));
	cBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cBufferDesc.ByteWidth = sizeof(TextureInfo) * this->amountOfTextures;
	cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cBufferDesc.MiscFlags = 0;
	cBufferDesc.StructureByteStride = 0;

	ZeroMemory(&data, sizeof(data));
	data.pSysMem = this->objTex.data();

	hr = gDevice->CreateBuffer(&cBufferDesc, &data, &shaderBuffer);

	std::cout << this->matList.at(0).daName;

	if (FAILED(hr))
	{
		return exit(-1);
	}
}

void OBJ::Texture(ID3D11Device* &gDevice, ID3D11DeviceContext* &gDeviceContext, ID3D11ShaderResourceView* &gTextureRTV)
{
	HRESULT hr;

	wchar_t test[256];

	for (int i = 0; i < 256; i++)
	{
		test[i] = this->matList.at(0).daName[i];
	}


	hr = DirectX::CreateWICTextureFromFile(gDevice, gDeviceContext, test, NULL, &gTextureRTV, NULL);

	if (FAILED(hr))
	{
		return exit(-1);
	}
}

int OBJ::ReturnVertexInfo()
{
	return sizeof(TriangleInfo);
}
int OBJ::getAmountOfVertecies()
{
	return this->amountOfVertecies;
}

const char* OBJ::returnDiffuseName()
{
	return this->matList.at(0).daName;
}
