#pragma once
#include "BBox.h"
#include "PressurePlate.h"
#include "Lever.h"
#include "FbxImport.h"
#include "Sound.h"

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


enum CollisionEnum
{
	eCollider = 0,
	eTrigger = 1
};

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
	int id;
	CollisionData() {
		collision = false;
		collisionType = -1;
		id = -1;
	}
};

class Collision
{
private:
	// Array with BBox objects for the collisions
	BBox* m_BoundingBox;
	// BBox object for the player
	BBox m_PlayerBox;
	// Number of bounding boxes in the scene
	int bbCount;
	// Collisiondata that hold info about collisions that happens
	CollisionData cData[2];
public:
	Collision();

	// Constructor for initializing bounding boxes
	// Parameters FBXImport::Mesh*& meshes, int meshCount
	Collision(FBXImport::Mesh*& meshes, int meshCount);
	~Collision();

	// This calculates collisions with bounding boxes and stores it in cData. 
	// cData is an array of 2 where 0 is for collider data and 1 is for trigger data
	CollisionData* calculateCollisionData(XMMATRIX& playerWorldMatrix, bool isDigging);

	// Returns the cData variable to access the collision data
	CollisionData* getCollisionData();

	// Removes a bounding box by id
	void removeBoundingBox(int id);

	void disableBoundingBox(int id);
	void enableBoundingBox(int id);

	BBox*& getBBoxArray();

	// Updates the player bounding box with the player world matrix
	void updatePlayerBB(XMMATRIX& playerWorldMatrix);
};



// Lever toggle on off
// Pressure plate time active - every one is different

// Todo:
// 1. Know if we step on a pressure plate and print it
// 2. Print out for as long as the pressure plate is active


// This class is used for pressure plates and levers. 

#define __id__(x) getIndexById(x)
#define E GetAsyncKeyState(0x45)
#define __id collisionData[eTrigger].id


class InteractiveCollision
{
private:

	bool flag, flag2, flag3, flag4, flag5, flag6, flag7;
	Lever* m_lever;
	PressurePlate* m_pressurePlate;

	int* index_by_id = nullptr;

	int pressurePlateCount = 0;
	int leverCount = 0;

	time_t tButtonPress;
	time_t lTimePress;

public:

	string test(CollisionData *collisionData, Collision& col, SoundManager& SM, float posX, float posZ);

	int getIndexById(int id);

	void populateIndexArray(FBXImport::Mesh* &meshes, int meshCount);
	
	// Might not be needed..
	void updateIndexArray();

	InteractiveCollision();
	InteractiveCollision(FBXImport::Mesh* &meshes, int meshCount);
	~InteractiveCollision();
};
