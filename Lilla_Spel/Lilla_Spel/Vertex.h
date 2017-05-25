//#pragma once
//
//#include <DirectXMath.h>
//
//
//using namespace DirectX;
//
//class Vertex {
//public:
//	float x, y, z, u, v;
//	//XMFLOAT4X4 WToT;
//
//	Vertex() {
//		x = y = z = u = v = 0;
//		//WToT = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
//	}
//
//	Vertex(float x, float y, float z, float u, float v) {
//		this->x = x; this->y = y; this->z = z; this->u = u; this->v = v;
//		//WToT = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
//	}
//};

struct Vertex {
	float position[3] = { 0, 0, 0 };
	float uv[2] = { 0, 0 };
	float normal[3] = { 0, 0, 0 };

	int jointIndices[8] = { -1, -1, -1, -1, -1, -1, -1, -1 };
	float jointWeights[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

	Vertex() {};

	Vertex(float x, float y, float z, float u, float v, float nx, float ny, float nz) {
		position[0] = x; position[1] = y; position[2] = z;
		uv[0] = u; uv[1] = v;
		normal[0] = nx; normal[1] = ny; normal[2] = nz;
	}
};
