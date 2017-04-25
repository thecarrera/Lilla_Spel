#pragma once
#include "BBox.h"
#include "PressurePlate.h"
#include "Lever.h"


//Todo:
// 1. Make bounding box appear ingame - done
// 2. Check collision against bb - done
// 3. Import bb data from file
// 4. Stop movement if collision is detected - done
// 5. Make player move / stand still based on collision type - done



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


	CollisionData calculateCollisionData(XMMATRIX playerWorldMatrix, bool isDigging);
	CollisionData& getCollisionData();
	void updatePlayerBB(XMMATRIX& playerWorldMatrix);
};



// Lever toggle on off
// Pressure plate time active - every one is different

// Todo:
// 1. Know if we step on a pressure plate and print it
// 2. Print out for as long as the pressure plate is active


// This class is used for pressure plates and levers. 
class InteractiveCollision
{
private:
	Lever* m_lever;
	PressurePlate* m_pressurePlate;

public:

	void test(CollisionData collisionData);

	InteractiveCollision();
	~InteractiveCollision();
};