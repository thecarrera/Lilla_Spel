#pragma once

#include <DirectXMath.h>


using namespace DirectX;

class Vertex {
public:
	float x, y, z, u, v;
	//XMFLOAT4X4 WToT;

	Vertex() {
		x = y = z = u = v = 0;
		//WToT = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	}

	Vertex(float x, float y, float z, float u, float v) {
		this->x = x; this->y = y; this->z = z; this->u = u; this->v = v;
		//WToT = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	}
};
