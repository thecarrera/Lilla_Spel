#pragma once

#pragma comment(lib, "windowscodecs.lib")

#include <iostream>
#include <vector>


#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

//#include <wincodec.h>
//#include <wrl.h>
//#include "WICTextureLoader.h"

#include "TriangleData.h"



#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

#define WLABEL "Lilla Spel Projekt"
#define HEIGHT 800
#define WIDTH 640
#define PIXELSAMPLE 4

#define SAFE_RELEASE(x) if(x) x->Release(), x = nullptr
#define SAFE_DELETE(x) if(x) delete[] x, x = nullptr

struct objMatrices
{
	DirectX::XMMATRIX worldM;
	DirectX::XMMATRIX viewM;
	DirectX::XMMATRIX projM;
};

struct ParticleType
{
	float PositionX;
	float PositionY;
	float PositionZ;
	float red;
	float green;
	float blue;
	float velocity;
	bool active;
};

struct VertexType
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 texture;
	DirectX::XMFLOAT4 color;
};