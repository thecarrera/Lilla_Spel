#pragma once

#pragma comment(lib, "windowscodecs.lib")

#include <iostream>
#include <vector>
#include <fstream>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

//#include <wincodec.h>
//#include <wrl.h>
#include "WICTextureLoader.h"

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

#define WLABEL "Lilla Spel Projekt"
#define HEIGHT 800
#define WIDTH 640
#define PIXELSAMPLE 4

#define SAFE_RELEASE(x) if(x) x->Release(), x = nullptr
#define SAFE_DELETE(x) if(x) delete[] x, x = nullptr