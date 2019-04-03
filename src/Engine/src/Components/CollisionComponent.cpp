#include "Components/CollisionComponent.h"


bool CollisionComponent::isBoundingBoxDirty() const
{
	return mBoundingBoxDirty;
}

const BoundingBox& CollisionComponent::getOriginalBoundingBox() const
{
	return mOriginalBoundingBox;
}

void CollisionComponent::setOriginalBoundingBox(const BoundingBox& originalBoundingBox)
{
	mOriginalBoundingBox = originalBoundingBox;
	mBoundingBoxDirty = false;
}

void CollisionComponent::setBoundingBox(const BoundingBox& boundingBox)
{
    mBoundingBox = boundingBox;
}

const Hull& CollisionComponent::getGeometry() const
{
	return mGeometry;
}

void CollisionComponent::setGeometry(const Hull& geometry)
{
	mGeometry = geometry;
	mBoundingBoxDirty = true;
}

const BoundingBox& CollisionComponent::getBoundingBox() const
{
	return mBoundingBox;
}
