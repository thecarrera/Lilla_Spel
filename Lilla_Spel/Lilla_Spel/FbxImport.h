#pragma once

#include "Includes.h"

#include <iostream>
#include <string>
#include <fstream>

class FBXImport
{
public:
	struct Vertex {
		float position[3];
		float uv[2];
		float normal[3];
	};

	class Mesh {
	public:
		Vertex* vertices = nullptr;
		int vertexCount = 0;
		int strLength;
		unsigned int vertSize = 0;
		std::string texturePath = "";

		// 0 - Not a Bounding Box
		// 1 - Above ground Bounding Box
		// 2 - Below ground Bounding Box
		// 3 - Above and Below ground Bounding Box
		// 4 - Pressure Plate Bounding Box
		// 5 - Lever Bounding Box
		int customAttribute = 0;

		~Mesh() {
			if (vertexCount != 0) {
				delete[] vertices;
			}
		};

		void addVertex() {
			vertices = new Vertex[this->vertexCount];
		}
	};

	class FBXData
	{
	public:
		Mesh* meshes = nullptr;
		int meshCount = 0;

		~FBXData()
		{
			if (meshCount > 0) {
				delete[] meshes;
			}
		};

		void AddMesh(int count) {
			Mesh* newA = new Mesh[count];

			meshes = newA;
		};
	};

private:
	FBXData* data;
	int fileCount = 0;
	int totalSumMeshes = 0;

private:
	void BindDataToBuffer(ID3D11Device* gDevice, ID3D11Buffer** &gVertexBufferArray, FBXData &FBX)
	{
		HRESULT hr;

		//if (gVertexBufferArray != nullptr)
		//{
		//	ID3D11Buffer** tempBuffer = new ID3D11Buffer*[this->totalSumMeshes + FBX.meshCount];
		//	tempBuffer = { nullptr };
		//
		//	for (int i = 0; i < this->totalSumMeshes; i++)
		//	{
		//		tempBuffer[i] = gVertexBufferArray[i];
		//	}

		//	gVertexBufferArray = tempBuffer;

		//	this->totalSumMeshes += FBX.meshCount;
		//}
		//else 
		//{
		gVertexBufferArray = new ID3D11Buffer*[FBX.meshCount];

		this->totalSumMeshes += FBX.meshCount;
		//}

		for (int i = 0; i < FBX.meshCount; i++)
		{
			D3D11_BUFFER_DESC bufferDesc;
			ZeroMemory(&bufferDesc, sizeof(bufferDesc));
			bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bufferDesc.Usage = D3D11_USAGE_DEFAULT;
			bufferDesc.ByteWidth = sizeof(Vertex) * FBX.meshes[i].vertexCount;

			D3D11_SUBRESOURCE_DATA data;
			ZeroMemory(&data, sizeof(data));
			data.pSysMem = &FBX.meshes[i].vertices[0];
			hr = gDevice->CreateBuffer(&bufferDesc, &data, &gVertexBufferArray[i]);

			if (FAILED(hr))
			{
				exit(-1);
			}
		}
	}

	void loadModel(std::string fileDir, ID3D11Device* gDevice, ID3D11Buffer**& gVertexBufferArray, int count)
	{
		std::ifstream is(fileDir, std::ios::binary);
		//char* dump;
		char* temp;

		if (!is.is_open())
		{
			std::cout << "Could not find the file!" << std::endl;
		}
		else
		{
			is.read((char*)&data[count].meshCount, sizeof(int));


			data[count].AddMesh(data[count].meshCount);

			for (int i = 0; i < data[count].meshCount; i++)
			{

				is.read((char*)&data[count].meshes[i].strLength, sizeof(int));
				std::cout << data[count].meshes[i].strLength << std::endl;

				temp = new char[data[count].meshes[i].strLength];

				is.read((char*)temp, data[count].meshes[i].strLength);

				temp[data[count].meshes[i].strLength + 1] = 0;

				data[count].meshes[i].texturePath = temp;

				is.read((char*)&data[count].meshes[i].vertexCount, sizeof(int));
				is.read((char*)&data[count].meshes[i].vertSize, sizeof(int));

				data[count].meshes[i].addVertex();

				is.read((char*)data[count].meshes[i].vertices, data[count].meshes[i].vertSize);

				std::cout << data[0].meshes[i].vertices[0].position[0] << std::endl;


				is.read((char*)&data[count].meshes[i].customAttribute, sizeof(int));

			}
			is.close();

			std::cout << data[count].meshes[0].texturePath << std::endl;

			this->BindDataToBuffer(gDevice, gVertexBufferArray, data[count]);

			std::cout << data[0].meshes[0].vertices[0].position[0] << std::endl;
		}
	};

	void addFile()
	{
		fileCount++;
		FBXData* temp = new FBXData[fileCount];

		if (fileCount <= 1)
		{
			data = temp;
		}
		else
		{
			for (int i = 0; i < fileCount; i++)
			{
				temp[i] = data[i];
			}
			data = temp;
		}
	};
	int returnMeshVertex(int i)
	{
		return this->data[0].meshes[i].vertexCount;
	}


public:
	FBXImport() {};
	~FBXImport() {
		if (fileCount > 0) {
			delete[] data;
		}
	};

	void Import(std::string fileDir, ID3D11Device* gDevice, ID3D11Buffer**& gVertexBufferArray)
	{
		this->addFile();

		this->loadModel(fileDir, gDevice, gVertexBufferArray, fileCount - 1);
	};

	int getPlayerSumVertices()
	{
		int sum = 0;


		sum += data[0].meshes[5].vertexCount;

		return sum;
	}
	int getSumVertices()
	{
		int sum = 0;

		for (int i = 0; i < fileCount; i++)
		{
			for (int j = 6; j < data[i].meshCount; j++)
			{
				sum += data[i].meshes[j].vertexCount;
			}
		}
		return sum;
	}
	int getMeshBoundingBox(int position)
	{
		return data[0].meshes[position].customAttribute;
	}

	int getTotalMeshes()
	{
		return this->totalSumMeshes;
	};

	Mesh*& getMeshes()
	{
		return data[0].meshes;
	}

	// Returns the number of meshes
	int getMeshCount()
	{
		return data[0].meshCount;
	}
	int getMeshVertexCount(int i)
	{
		return this->returnMeshVertex(i);
	}
};
