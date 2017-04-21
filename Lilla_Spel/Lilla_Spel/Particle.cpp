#include "Particle.h"

Particle::Particle()
{

}

Particle::~Particle()
{

}

void Particle::CreateTriangleData()
{
	struct TriangleVertex
	{
		float x, y, z;
	};

	TriangleVertex triangleVertices[12] =
	{
		//första triangeln
		-0.5f, -0.5f, 0.5f,

		0.0f, 0.5f, 0.0f,

		0.0f, -0.5f, -0.5f,

		//2 triangeln
		-0.5f, -0.5f, 0.5f,

		0.0f, 0.5f, 0.0f,

		0.5f, -0.5f, 0.5f,

		//3 triangle
		0.0f, -0.5f, -0.5f,

		0.0f, 0.5f, 0.0f,

		0.5f, -0.5f, 0.5f,

		//4 triangle
		-0.5f, -0.5f, 0.5f,

		0.5f, -0.5f, 0.5f,

		0.0f, -0.5f, -0.5f,
	};

	//D3D11_BUFFER_DESC bufferDesc;
	//memset(&bufferDesc, 0, sizeof(bufferDesc)); //zeroar memoriet
	//bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	//bufferDesc.ByteWidth = sizeof(triangleVertices);

	//D3D11_SUBRESOURCE_DATA data;
	//data.pSysMem = triangleVertices;
	//gDevice->CreateBuffer(&bufferDesc, &data, &gVertexBuffer);
}