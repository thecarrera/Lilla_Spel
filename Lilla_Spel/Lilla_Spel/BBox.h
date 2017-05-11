#pragma once
#include <DirectXCollision.h>
#include <iostream>
using namespace std;

using namespace DirectX;
class BBox
{
private:
	int m_CollisionType;
	int m_id;
	int m_originalColType;
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

	// Sets the id
	void setId(int id);

	int getId() const;

	void setOriginalColType(int colType);

	int getOriginalColType() const;

	// Returns the BoundingBox
	BoundingBox& getBoundingBox();

	void createBoundingBoxFromCorners(XMFLOAT3* corners);

};