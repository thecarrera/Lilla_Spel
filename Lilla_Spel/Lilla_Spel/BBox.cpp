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

void BBox::setId(int id)
{
	this->m_id = id;
}

int BBox::getId() const
{
	return m_id;
}

void BBox::setOriginalColType(int colType)
{
	this->m_originalColType = colType;
}

int BBox::getOriginalColType() const
{
	return m_originalColType;
}

BoundingBox& BBox::getBoundingBox()
{
	return m_BoundingBox;
}

void BBox::createBoundingBoxFromCorners(XMFLOAT3 * corners)
{

	this->m_BoundingBox.CreateFromPoints(m_BoundingBox, 8, corners, sizeof(XMFLOAT3));
}
