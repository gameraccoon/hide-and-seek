#include "Components/CollisionComponent.h"

#include <nlohmann/json.hpp>

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

void CollisionComponent::toJson(nlohmann::json& outJson) const
{
	to_json(outJson, *this);
}

void to_json(nlohmann::json& outJson, const CollisionComponent& collision)
{
	outJson = nlohmann::json{
		{"geometry", collision.mGeometry}
	};
}

void from_json(const nlohmann::json& json, CollisionComponent& outCollision)
{
	json.at("geometry").get_to(outCollision.mGeometry);
}
