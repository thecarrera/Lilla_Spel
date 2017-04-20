#pragma once
#include "BBox.h"


//Todo:
// 1. Make bounding box appear ingame - done
// 2. Check collision against bb - done
// 3. Import bb data from file
// 4. Stop movement if collision is detected - done
// 5. Make player move / stand still based on collision type



// Collision types
// 0 - Mesh
// 1 - Above
// 2 - Below
// 3 - Both 
// 4 - Pressure plate
// 5 - Lever

// Defines
/*****************************************************************************************/
#define isCollidingWithPlayer getBoundingBox().Intersects(m_PlayerBox.getBoundingBox())
#define collisionMesh getCollisionType() == 0
#define collisionAbove getCollisionType() == 1
#define collisionBelow getCollisionType() == 2
#define collisionBoth getCollisionType() == 3
#define collisionPressurePlate getCollisionType() == 4
#define collisionLever getCollisionType() == 5
/*****************************************************************************************/

// Custom data type for collision. This lets us also specify which type the player is colliding with, giving us 
// more possibilities for customization
struct CollisionData
{
	bool collision;
	int collisionType;

	CollisionData() {
		collision = false;
		collisionType = -1;
	}
};

class Collision
{
private:
	BBox* m_BoundingBox;
	BBox m_PlayerBox;

	CollisionData collisionData;
public:
	Collision();
	~Collision();


	CollisionData getColllisionData(XMMATRIX trans, bool isDigging);

	void updatePlayerBB(XMMATRIX& trans);
};