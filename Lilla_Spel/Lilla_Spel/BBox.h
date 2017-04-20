#pragma once
#include <DirectXCollision.h>
#include <iostream>
using namespace std;

using namespace DirectX;
class BBox
{
private:
	int m_CollisionType;
	BoundingBox m_BoundingBox;
public:
	BBox();
	virtual ~BBox();

	
	// This sets the boundingbox for the object
	// Parameters A BoundingBox bbox 
	void setBoundingBox(BoundingBox bbox);

	// Sets the collision type
	void setCollisionType(int type);
	// Gets the collision type of the bounding box
	int getCollisionType() const;

	// Returns the BoundingBox
	BoundingBox& getBoundingBox();

};