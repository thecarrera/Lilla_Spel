#pragma once

#pragma comment(lib, "windowscodecs.lib")

#include <vector>


#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>

//#include <wincodec.h>
//#include <wrl.h>
//#include "WICTextureLoader.h"

#include "TriangleData.h"
#include "FbxImport.h"

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

#define WLABEL "Lilla Spel Projekt"
#define HEIGHT 800
#define WIDTH 640
#define PIXELSAMPLE 4

#define SAFE_RELEASE(x) if(x) x->Release(), x = nullptr
#define SAFE_DELETE(x) if(x) delete[] x, x = nullptr
//#define SAFE_LOOP2_DELETE(x,y) if(x) for(int i = 0; i < y; i++){if(x[i]) delete x[i]; } delete[] x, x = nullptr

struct objMatrices
{
	DirectX::XMMATRIX worldM;
	DirectX::XMMATRIX viewM;
	DirectX::XMMATRIX projM;
};