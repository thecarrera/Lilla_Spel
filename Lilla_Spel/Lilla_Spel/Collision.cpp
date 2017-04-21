#include "Collision.h"



Collision::Collision()
{
	// Test variable, this is the center of the cube
	XMFLOAT3 center = { -29.76096f, 0.0f, -2.32647f };

	XMFLOAT3 extent = { 1.0f, 1.0f, 1.0f };

	m_BoundingBox = new BBox[1];

	m_BoundingBox[0].setBoundingBox(BoundingBox(center, extent));
	m_BoundingBox[0].setCollisionType(4);

	center = { -0.058672f, 2.26698f, 0.451068f };

	m_PlayerBox.setBoundingBox(BoundingBox(center, extent));
}


Collision::~Collision()
{
}

CollisionData Collision::getColllisionData(XMMATRIX playerWorldMatrix, bool isDigging)
{
	
	updatePlayerBB(playerWorldMatrix);

	//cout << temp._14 << "   " << temp._34 << endl;
	//cout << m_BoundingBox[0].getBoundingBox().Center.x << "  " << m_BoundingBox[0].getBoundingBox().Center.z << endl;
	for (int i = 0; i < 1; i++)
	{
		// Check if player is burried and check if he is colliding with either "belowCollider" or "above and below collider"
		if (isDigging && (m_BoundingBox[i].collisionBelow || m_BoundingBox[i].collisionBoth))
			 m_BoundingBox[i].isCollidingWithPlayer ? collisionData.collision = true : collisionData.collision = false;
		
		// If player is not burried, but still colliding, check if its a pressure plate, lever or above collision
		else if (!isDigging && m_BoundingBox[i].isCollidingWithPlayer)
		{
			if (m_BoundingBox[i].collisionPressurePlate) {
				collisionData.collision = false;
				// Pressure plate type has id 4
				collisionData.collisionType = 4;
			}
			else if (m_BoundingBox[i].collisionLever) {
				collisionData.collision = false;
				collisionData.collisionType = 5;
			}
			else if(m_BoundingBox[i].collisionAbove){
				collisionData.collision = true;
			}
			else if (m_BoundingBox[i].collisionBoth) {
				collisionData.collision = true;
			}
			else {
				collisionData.collision = false;
				collisionData.collisionType = -1;
			}
		}
		else
		{
			collisionData.collision = false;
			collisionData.collisionType = -1;
		}
	}

	return collisionData;
}

void Collision::updatePlayerBB(XMMATRIX& playerWorldMatrix)
{
	XMFLOAT4X4 temp;
	XMStoreFloat4x4(&temp, playerWorldMatrix);

	// Update player bb _14, _34
	m_PlayerBox.getBoundingBox().Center = XMFLOAT3{ temp._14, 0, temp._34 };
}
