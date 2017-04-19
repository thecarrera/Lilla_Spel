#include "Collision.h"



Collision::Collision()
{
	// Test variable, this is the center of the cube
	XMFLOAT3 center = { -29.76096f, 0.0f, -2.32647f };

	XMFLOAT3 extent = { 1.0f, 1.0f, 1.0f };

	m_BoundingBox = new BoundingBox[1];

	m_BoundingBox[0] = BoundingBox(center, extent);


	center = { -0.058672f, 2.26698f, 0.451068f };

	m_PlayerBox = BoundingBox(center, extent);
}


Collision::~Collision()
{
}

bool Collision::TestCollision(XMMATRIX trans)
{
	updatePlayerBB(trans);
	//cout << temp._14 << "   " << temp._34 << endl;
	//cout << m_BoundingBox[0].Center.x << "  " << m_BoundingBox[0].Center.z << endl;

	if (m_BoundingBox[0].Intersects(m_PlayerBox))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Collision::updatePlayerBB(XMMATRIX trans)
{

	XMFLOAT4X4 temp;
	XMStoreFloat4x4(&temp, trans);

	// Update player bb _14, _34
	m_PlayerBox.Center = XMFLOAT3{ temp._14, 0, temp._34 };
}
