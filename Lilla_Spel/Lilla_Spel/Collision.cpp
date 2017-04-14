#include "Collision.h"



Collision::Collision()
{
	// Test variable, this is the center of the cube
	XMFLOAT3 center = { 29.76096f, 1.83139f, -2.32647f };

	XMFLOAT3 extent = { 1,1,1 };

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
	m_BoundingBox[0].Transform(m_BoundingBox[0], trans);


	if (m_BoundingBox[0].Intersects(m_PlayerBox))
	{
		return true;
	}
	else
	{
		return false;
	}
}
