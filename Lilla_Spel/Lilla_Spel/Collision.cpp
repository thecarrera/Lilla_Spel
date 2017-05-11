#include "Collision.h"
#include <mutex>

once_flag flag;


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
				
				if (meshes[i].id == 3)
				{
					int z = 0;
					int x = 5;
				}

				m_BoundingBox[bbIndex].createBoundingBoxFromCorners(cornerArray);
				m_BoundingBox[bbIndex].setCollisionType(meshes[i].customAttribute);
				m_BoundingBox[bbIndex].setOriginalColType(meshes[i].customAttribute);
				m_BoundingBox[bbIndex].setId(meshes[i].id);


				cout << "id: " << m_BoundingBox[bbIndex].getId() << " Coltype: " << m_BoundingBox[bbIndex].getCollisionType() << endl;
				bbIndex++;

		}
	}


	XMFLOAT3 extent = { 0.5f, 0.5f, 0.5f };


	/*m_BoundingBox[1].setBoundingBox(BoundingBox(center, extent));
	m_BoundingBox[1].setCollisionType(4);*/

	XMFLOAT3 center = { 0.0, 0.0f, 0.0f };
	

	m_PlayerBox.setBoundingBox(BoundingBox(center, extent));
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
		if (isDigging && (m_BoundingBox[i].collisionBelow || m_BoundingBox[i].collisionBoth)) {
			if (m_BoundingBox[i].isCollidingWithPlayer) {
				cData[eCollider].collision = true;
				colCount++;
			}
		}

		if (isDigging && m_BoundingBox[i].collisionAbove)
		{
			if (m_BoundingBox[i].isCollidingWithPlayer) {
				cout << "Collision. i is: " << i << endl;
				cData[eCollider].collisionType = 1;
				colCount++;
			}
		}
		// If player is not burried, but still colliding, check if its a pressure plate, lever or above collision
		else if (!isDigging && m_BoundingBox[i].isCollidingWithPlayer)
		{
			if (m_BoundingBox[i].collisionPressurePlate) {
				cData[eTrigger].collision = false;
				// Pressure plate type has id 4
				cData[eTrigger].collisionType = 4;
				cData[eTrigger].id = m_BoundingBox[i].getId();
				triggerCount++;
			}
			else if (m_BoundingBox[i].collisionLever) {
				cData[eTrigger].collision = false;
				cData[eTrigger].collisionType = 5;
				cData[eTrigger].id = m_BoundingBox[i].getId();
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
				cData[eTrigger].id = -1;
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
				cData[eTrigger].id = -1;
			}
		}
	}

	return cData;
}

CollisionData* Collision::getCollisionData()
{
	return this->cData;
}

void Collision::removeBoundingBox(int id)
{
	cout << "Removing bounding box " << id  << endl;

	for (int i = 0; i < bbCount; i++)
	{
		cout << "id: " << " index: " << i << " " << m_BoundingBox[i].getId() << " Coltype: " << m_BoundingBox[i].getCollisionType() << endl;
	}


	BBox* tmp = new BBox[bbCount - 1];
	int tmpIndex = 0;
	for (int i = 0; i < bbCount; i++)
	{
		if (m_BoundingBox[i].getId() != id)
		{
			tmp[tmpIndex] = m_BoundingBox[i];
			tmpIndex++;
		}
	}
	bbCount--;
	delete[] m_BoundingBox;

	m_BoundingBox = tmp;
}

void Collision::disableBoundingBox(int id)
{
	for (int i = 0; i < bbCount; i++)
	{
		if (m_BoundingBox[i].getId() == id)
		{
			m_BoundingBox[i].setOriginalColType(m_BoundingBox[i].getCollisionType());
			m_BoundingBox[i].setCollisionType(0);
		}
	}
}

void Collision::enableBoundingBox(int id)
{
	for (int i = 0; i < bbCount; i++)
	{
		if (m_BoundingBox[i].getId() == id)
		{
			m_BoundingBox[i].setCollisionType(m_BoundingBox[i].getOriginalColType());
		}
	}
}

BBox *& Collision::getBBoxArray()
{
	return m_BoundingBox;
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

void InteractiveCollision::test(CollisionData* collisionData, Collision& col)
{

	// Check for the pressure plate collision
	if (collisionData[eTrigger].id == 2) {
		m_pressurePlate[__id__(2)].togglePressurePlate();
		//cout << "pressure plate with id " << m_pressurePlate[__id__(2)].getId() << endl;
	}
	else if (E && __id == 0) {
			m_lever[__id__(0)].activateLever();
	}


	if (m_lever[__id__(0)].getLeverOnOffState())
	{
		// turn off boundingbox based on id 
		call_once(flag, [&]() { col.removeBoundingBox(1); }
		);		
	}

	if (m_pressurePlate[__id__(2)].getPressurePlateData().toggled)
	{
		//cout << "pressureplate toggled" << endl;
		col.disableBoundingBox(3);
	}
	else
	{
		col.enableBoundingBox(3);
	}

	//if (m_pressurePlate[__id__(2)].getPressurePlateData().active && !m_pressurePlate[__id__(2)].getPressurePlateData().ticking) {
	//	col.disableBoundingBox(3);
	//}
	//else {
	//	//col.enableBoundingBox(3);
	//}

}
int InteractiveCollision::getIndexById(int id)
{
	return index_by_id[id];
}

InteractiveCollision::InteractiveCollision()
{
}

InteractiveCollision::InteractiveCollision(FBXImport::Mesh* &meshes, int meshCount)
{
	populateIndexArray(meshes, meshCount);

	pressurePlateCount = 0;
	leverCount = 0;

	for (int i = 0; i < meshCount; i++)
	{
		if (meshes[i].customAttribute == 4)
		{
			pressurePlateCount++;
		}
		else if (meshes[i].customAttribute == 5) {
			leverCount++;
		}
	}

	m_pressurePlate = new PressurePlate[pressurePlateCount];
	m_lever = new Lever[leverCount];


	int ppCount = 0;
	// Hardcoded for now
	for (int i = 0; i < meshCount; i++)
	{
		if (meshes[i].customAttribute == 4)
		{
			m_pressurePlate[ppCount] = PressurePlate(3000);
			m_pressurePlate[ppCount].setId(meshes[i].id);
			index_by_id[meshes[i].id] = ppCount;
			ppCount++;
		}

	}

	int lCount = 0;
	// Hardcoded for now
	for (int i = 0; i < meshCount; i++)
	{
		if (meshes[i].customAttribute == 5)
		{
			m_lever[lCount] = Lever();
			m_lever[lCount].setId(meshes[i].id);
			index_by_id[meshes[i].id] = lCount;
			lCount++;
		}

	}
}

InteractiveCollision::~InteractiveCollision()
{
}
void InteractiveCollision::populateIndexArray(FBXImport::Mesh* &meshes, int meshCount)
{
	// Fill index id array
	int idCount = 0;
	for (int i = 0; i < meshCount; i++)
	{
		if (meshes[i].id > -1)
		{
			idCount++;
		}
	}

	index_by_id = new int[idCount];
}


// Might not be needed...
void InteractiveCollision::updateIndexArray()
{
	for (int i = 0; i < pressurePlateCount; i++)
	{
		index_by_id[m_pressurePlate[i].getId()] = i;
	}

	for (int i = 0; i < pressurePlateCount; i++)
	{
		index_by_id[m_lever[i].getId()] = i;
	}

}
