#include "Importers.h"

OBJHandler::OBJHandler()
{
	this->amountOfMeshes = 0;
	this->amountOfTextures = 0;
}
OBJHandler::~OBJHandler()
{
	this->Clean();
}
void OBJHandler::Clean()
{
}

void OBJHandler::LoadModel(std::string fileName, ID3D11Device* gDevice, ID3D11Buffer* &gVertexBuffer, ID3D11Buffer* &shaderBuffer)
{
	this->amountOfMeshes = this->findAmountOfMeshes(fileName);

	std::ifstream ss(fileName, std::ifstream::in);

	char cmd[256] = { 0 };
	char mtlName[256] = { 0 };
	int tempV = 0;

	DirectX::XMFLOAT3 inputValues;
	DirectX::XMFLOAT2 inputTex;
	DirectX::XMINT3 temp_vert[3];

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

			this->amountOfTextures = this->findAmountOfTextures(mtlName);

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

					meshTex.at(this->amountOfTextures).setMatName(cmd);

					this->amountOfTextures++;

				if (strcmp(cmd, "Kd") == 0)
				{
					mtl >> inputValues.x >> inputValues.y >> inputValues.z;

					meshTex.at(this->amountOfTextures - 1).setDiffValues(inputValues);
				}
				else if (strcmp(cmd, "Ka") == 0)
				{
					mtl >> inputValues.x >> inputValues.y >> inputValues.z;

					meshTex.at(this->amountOfTextures - 1).setAmbValues(inputValues);
				}
				else if (strcmp(cmd, "Ks") == 0)
				{
					mtl >> inputValues.x >> inputValues.y >> inputValues.z;

					meshTex.at(this->amountOfTextures - 1).setSpecValues(inputValues);
				}
				else if (strcmp(cmd, "map_Kd") == 0 || strcmp(cmd, "map_Ka") == 0)
				{
					mtl >> cmd;
					this->meshTex.at(this->amountOfTextures- 1).setDiffAmbName(cmd);
				}
				//else if (strcmp(cmd, "map_Ks"))
				//{
				//	mtl >> matL.spName;
				//}
				//else if (strcmp(cmd, "map_bump") || strcmp(cmd, "bump"))
				//{
				//	mtl >> matL.bumpName;
				//}

				}
			}
		}
		else if (strcmp(cmd, "g") == 0)
		{
			ss >> cmd;
			this->mesh.at(this->amountOfMeshes).setGroupName(cmd);
			this->amountOfMeshes++;
		}
		else if (strcmp(cmd, "v") == 0)
		{
			ss >> inputValues.x >> inputValues.y >> inputValues.z;
			inputValues.z = (inputValues.z * -1.0f);

			this->mesh.at(this->amountOfMeshes - 1).addVertexPos(inputValues);
		}
		else if (strcmp(cmd, "vt") == 0)
		{
			ss >> inputTex.x >> inputTex.y;

			this->mesh.at(this->amountOfMeshes - 1).addVertexUV(inputTex);
		}
		else if (strcmp(cmd, "vn") == 0)
		{
			ss >> inputValues.x >> inputValues.y >> inputValues.z;
			inputValues.z = (inputValues.z * -1.0f);

			this->mesh.at(this->amountOfMeshes - 1).addVertexNorm(inputValues);

		}
		else if (strcmp(cmd, "f") == 0)
		{
			for (int i = 3; i > 0; i--)
			{
				ss >> temp_vert[i - 1].x >> trash >> temp_vert[i - 1].y >> trash >> temp_vert[i - 1].z;
			}

			for (int i = 0; i < 3; i++)
			{
				this->mesh.at(this->amountOfMeshes - 1).createFaces(temp_vert[i - 1], tempV);
				tempV++;
			}
		}
	}

	HRESULT hr;

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(this->mesh.at(0).returnVertexInfo()) * this->getAmountOfVertecies();

	D3D11_SUBRESOURCE_DATA data;
	for (int i = 0; i < this->amountOfMeshes; i++)
	{
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = this->mesh.at(i).returnVertexData();
		hr = gDevice->CreateBuffer(&bufferDesc, &data, &gVertexBuffer);
		if (FAILED(hr))
		{
			return exit(-1);
		}
	}
	D3D11_BUFFER_DESC cBufferDesc;
	ZeroMemory(&cBufferDesc, sizeof(cBufferDesc));
	cBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cBufferDesc.ByteWidth = this->meshTex.at(0).returnTextureInfo() * this->amountOfTextures;
	cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cBufferDesc.MiscFlags = 0;
	cBufferDesc.StructureByteStride = 0;

	for (int i = 0; i < this->amountOfTextures; i++)
	{
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = this->meshTex.data();

		hr = gDevice->CreateBuffer(&cBufferDesc, &data, &shaderBuffer);
	}
	std::cout << this->meshTex.at(0).returnDiffuseName();

	if (FAILED(hr))
	{
		return exit(-1);
	}
}
void OBJHandler::Texture(ID3D11Device* &gDevice, ID3D11DeviceContext* &gDeviceContext, ID3D11ShaderResourceView* &gTextureRTV)
{
	HRESULT hr;

	wchar_t mat[256];


	for (int i = 0; i < this->amountOfTextures; i++)
	{
		for (int j = 0; j < 256; j++)
		{
			mat[j] = this->meshTex.at(i).returnDiffuseName()[j];
		}
	}

	hr = DirectX::CreateWICTextureFromFile(gDevice, gDeviceContext, mat, NULL, &gTextureRTV, NULL);

	if (FAILED(hr))
	{
		return exit(-1);
	}
}

int OBJHandler::findAmountOfMeshes(std::string fileName)
{
	std::ifstream ss(fileName, std::ifstream::in);

	char cmd[256] = { 0 };
	int tmp = 0;

	while (ss.good())
	{
		if (!ss)
		{
			break;
		}
		else if (strcmp(cmd, "g default") == 0)
		{
			tmp += 1;
		}
	}

	return tmp;
}
int OBJHandler::findAmountOfTextures(std::string fileName)
{
	std::ifstream ss(fileName, std::ifstream::in);

	char cmd[256] = { 0 };
	int tmp = 0;

	while (ss.good())
	{
		if (!ss)
		{
			break;
		}
		else if (strcmp(cmd, "newmtl") == 0)
		{
			tmp += 1;
		}
	}

	return tmp;
}

int OBJHandler::getAmountOfVertecies()
{
	int sum = 0;
	for (int i = 0; i < this->amountOfMeshes; i++)
	{
		sum += this->mesh.at(i).getAmountOfVertecies();
	}
	return sum;
}
UINT32 OBJHandler::returnVertexInfo()
{
	return this->mesh.at(0).returnVertexInfo();
}
const char* OBJHandler::returnDiffuseName()
{
	return this->meshTex.at(0).returnDiffuseName();
}