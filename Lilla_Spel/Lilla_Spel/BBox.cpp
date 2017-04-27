#include "BBox.h"



BBox::BBox()
{
}


BBox::~BBox()
{
}


void BBox::setBoundingBox(BoundingBox bbox)
{
	this->m_BoundingBox = bbox;
}

void BBox::setCollisionType(int type)
{
	this->m_CollisionType = type;
}

int BBox::getCollisionType() const
{
	return this->m_CollisionType;
}

BoundingBox& BBox::getBoundingBox()
{
	return m_BoundingBox;
}

void BBox::createBoundingBoxFromCorners(XMFLOAT3 * corners)
{

	this->m_BoundingBox.CreateFromPoints(m_BoundingBox, 8, corners, sizeof(XMFLOAT3));
}
