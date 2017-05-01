#include "Collision.h"

Collision::Collision() {

}

Collision::Collision(FBXImport::Mesh* &meshes, int meshCount)
{

		/*
		Far:
		Top left : 2,
		Bot left : 0,
		Bot right : 1,
		Top right : 5,

		Near:
		Top left : 8,
		Bot left : 14,
		Top right : 11,
		Bot right : 17,
		*/


	bbCount = 0;

	for (int i = 0; i < meshCount; i++)
	{
		if (meshes[i].customAttribute > 0)
		{
			bbCount++;
		}

	}

	m_BoundingBox = new BBox[bbCount];

	XMFLOAT3 cornerArray[8];

	int bbIndex = 0;
	for (int i = 0; i < meshCount; i++)
	{
		if (meshes[i].customAttribute > 0)
		{
		
				// Top left Near
				cornerArray[0].x = meshes[i].vertices[8].position[0];
				cornerArray[0].y = meshes[i].vertices[8].position[1];
				cornerArray[0].z = meshes[i].vertices[8].position[2];

				// Top right Near
				cornerArray[1].x = meshes[i].vertices[11].position[0];
				cornerArray[1].y = meshes[i].vertices[11].position[1];
				cornerArray[1].z = meshes[i].vertices[11].position[2];
				
				// Bot right near
				cornerArray[2].x = meshes[i].vertices[17].position[0];
				cornerArray[2].y = meshes[i].vertices[17].position[1];
				cornerArray[2].z = meshes[i].vertices[17].position[2];
				// Bot left near
				cornerArray[3].x = meshes[i].vertices[14].position[0];
				cornerArray[3].y = meshes[i].vertices[14].position[1];
				cornerArray[3].z = meshes[i].vertices[14].position[2];

				// top left far
				cornerArray[4].x = meshes[i].vertices[2].position[0];
				cornerArray[4].y = meshes[i].vertices[2].position[1];
				cornerArray[4].z = meshes[i].vertices[2].position[2];
				// top right far
				cornerArray[5].x = meshes[i].vertices[5].position[0];
				cornerArray[5].y = meshes[i].vertices[5].position[1];
				cornerArray[5].z = meshes[i].vertices[5].position[2];
				// bot right far
				cornerArray[6].x = meshes[i].vertices[1].position[0];
				cornerArray[6].y = meshes[i].vertices[1].position[1];
				cornerArray[6].z = meshes[i].vertices[1].position[2];
				// bot left far
				cornerArray[7].x = meshes[i].vertices[0].position[0];
				cornerArray[7].y = meshes[i].vertices[0].position[1];
				cornerArray[7].z = meshes[i].vertices[0].position[2];
			
				m_BoundingBox[bbIndex].createBoundingBoxFromCorners(cornerArray);
				m_BoundingBox[bbIndex].setCollisionType(meshes[i].customAttribute);
				m_BoundingBox[bbIndex].setId(meshes[i].id);
				bbIndex++;

		}
	}


	XMFLOAT3 extent = { 1.0f, 1.0f, 1.0f };


	/*m_BoundingBox[1].setBoundingBox(BoundingBox(center, extent));
	m_BoundingBox[1].setCollisionType(4);*/

	XMFLOAT3 center = { -0.058672f, 2.26698f, 0.451068f };
	XMFLOAT4 orientation = { 0, 0, 0, 1 };

	m_PlayerBox.setBoundingBox(BoundingOrientedBox(center, extent, orientation));
}


Collision::~Collision()
{
}

CollisionData* Collision::calculateCollisionData(XMMATRIX playerWorldMatrix, bool isDigging)
{
	int colCount = 0;
	int triggerCount = 0;
	
	
	updatePlayerBB(playerWorldMatrix);

	for (int i = 0; i < bbCount; i++)
	{
		// Check if player is burried and check if he is colliding with either "belowCollider" or "above and below collider"
		if (isDigging && (m_BoundingBox[i].collisionBelow || m_BoundingBox[i].collisionBoth))
			if (m_BoundingBox[i].isCollidingWithPlayer) {
				cData[eCollider].collision = true;
				colCount++;
			}
			else {
				cData[eCollider].collision = false;
			}
		
		// If player is not burried, but still colliding, check if its a pressure plate, lever or above collision
		else if (!isDigging && m_BoundingBox[i].isCollidingWithPlayer)
		{
			if (m_BoundingBox[i].collisionPressurePlate) {
				cData[eTrigger].collision = false;
				// Pressure plate type has id 4
				cData[eTrigger].collisionType = 4;
				triggerCount++;
			}
			else if (m_BoundingBox[i].collisionLever) {
				cData[eTrigger].collision = false;
				cData[eTrigger].collisionType = 5;
				triggerCount++;
			}
			else if(m_BoundingBox[i].collisionAbove){
				cData[eCollider].collision = true;
				colCount++;
			}
			else if (m_BoundingBox[i].collisionBoth) {
				cData[eCollider].collision = true;
				colCount++;
			}
			else {
				cData[eCollider].collision = false;
				cData[eCollider].collisionType = -1;
				cData[eTrigger].collisionType = -1;
			}
		}
		else
		{
			if (colCount == 0) {
				cData[eCollider].collision = false;
				cData[eCollider].collisionType = -1;
			}
			if (triggerCount == 0) {
				cData[eTrigger].collision = false;
				cData[eTrigger].collisionType = -1;
			}
		}
	}

	return cData;
}

CollisionData* Collision::getCollisionData()
{
	return this->cData;
}

void Collision::updatePlayerBB(XMMATRIX& playerWorldMatrix)
{
	XMFLOAT4X4 temp;
	XMStoreFloat4x4(&temp, playerWorldMatrix);

	// Update player bb _14, _34
	m_PlayerBox.getBoundingBox().Center = XMFLOAT3{ temp._14, 0, temp._34 };
}

/*****************************************************************************************/
/*****************************************************************************************/
/*****************************************************************************************/

void InteractiveCollision::test(CollisionData* collisionData)
{

	// Check for the pressure plate collision
	if (collisionData[eTrigger].collisionType == 4) {
		m_pressurePlate[eCollider].activatePressurePlate();
		//cout << "Activating pressure plate!" << endl;
	}
	else if (collisionData[eTrigger].collisionType == 5 && GetAsyncKeyState(0x45)) {
		m_lever[eCollider].activateLever();
	}

	if (m_pressurePlate[0].getPressurePlateData().active && !m_pressurePlate[0].getPressurePlateData().ticking) {
		//cout << "plate is active" << endl;
	}

}
InteractiveCollision::InteractiveCollision()
{
}

InteractiveCollision::InteractiveCollision(FBXImport::Mesh* &meshes, int meshCount)
{
	int pressurePlateCount = 0;
	int leverCount = 0;

	for (int i = 0; i < meshCount; i++)
	{
		if (meshes->customAttribute == 4)
		{
			pressurePlateCount++;
		}
		else if (meshes->customAttribute == 5) {
			leverCount++;
		}
	}

	m_pressurePlate = new PressurePlate[1];
	m_lever = new Lever[1];



	// Hardcoded for now
	//m_pressurePlate = new PressurePlate[1];
	m_pressurePlate[0] = PressurePlate(3000);

	//m_lever = new Lever[1];
	m_lever[0] = Lever();
}

InteractiveCollision::~InteractiveCollision()
{
}
