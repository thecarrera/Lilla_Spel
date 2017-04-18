#pragma once
#include <DirectXCollision.h>
#include <iostream>
using namespace std;

using namespace DirectX;

//Todo:
// 1. Make bounding box appear ingame - done
// 2. Check collision against bb

class Collision
{
private:
	BoundingBox* m_BoundingBox;
	BoundingBox m_PlayerBox;
public:
	Collision();
	~Collision();


	bool TestCollision(XMMATRIX trans);

	void updatePlayerBB();
};