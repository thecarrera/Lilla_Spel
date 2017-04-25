#pragma once

#include <iostream>
#include <string>
#include <fstream>

class FBXImport
{
private:
	struct Vertex {
		float position[3];
		float uv[2];
		float normal[3];
	};

	class Mesh {
	public:
		Vertex* vertices;
		int vertexCount = 0;
		int strLength;
		std::string texturePath;

		~Mesh() {
			if (vertexCount != 0) {
				delete[] vertices;
			}
		};

		void addVertex(int count) {
			Vertex* newA = new Vertex[count];

			vertices = newA;
		}
	};

	class FBXData
	{
	public:
		Mesh* meshes;
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

	void BindDataToBuffer(ID3D11Device* gDevice, ID3D11Buffer** &gVertexBufferArray, int vertexcount, Vertex* &vertices)
	{
		HRESULT hr;

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		memset(&bufferDesc, 0, sizeof(bufferDesc));
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = sizeof(Vertex) * vertexcount;
		
		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = vertices;
		hr = gDevice->CreateBuffer(&bufferDesc, &data, gVertexBufferArray);

		if (FAILED(hr))
		{
			exit(-1);
		}

		getchar();
	}

	FBXData data;

public:
	void loadModel(std::string fileDir, ID3D11Device* gDevice, ID3D11Buffer**& gVertexBufferArray)
	{

		std::ifstream is(fileDir, std::ios::binary);

		is.read((char*)&data.meshCount, sizeof(int));

		std::cout << data.meshCount << std::endl;

		data.AddMesh(data.meshCount);

		for (int i = 0; i < data.meshCount; i++)
		{
			is.read((char*)&data.meshes[i].vertexCount, sizeof(int));
			is.read((char*)&data.meshes[i].strLength, sizeof(int));
			is.read((char*)(&data.meshes[i].texturePath), data.meshes[i].strLength);

			data.meshes[i].addVertex(data.meshes[i].vertexCount);

			for (int j = 0; j < data.meshes[i].vertexCount; j++)
			{
				is.read((char*)&data.meshes[i].vertices[j], sizeof(Vertex));
			}
			
			this->BindDataToBuffer(gDevice, gVertexBufferArray, this->data.meshes[i].vertexCount, data.meshes[i].vertices);
		}
		is.close();
	
	};	

	int getSumVertices() {
		int sum;

		for (int i = 0; i < data.meshCount; i++)
		{
			sum += data.meshes[i].vertexCount;
		}
		return sum;
	}
};