#pragma once

#include "Core/Component.h"

#include <Structures/Hull.h>
#include <Structures/BoundingBox.h>

/**
 * Component that stores information and logic about position and movement
 */
class CollisionComponent : public BaseComponent
{
public:
	const Hull& getGeometry() const;
	void setGeometry(const Hull& geometry);

	const BoundingBox& getBoundingBox() const;
	void setBoundingBox(const BoundingBox& newBoundingBox);

	bool isBoundingBoxDirty() const;

	const BoundingBox& getOriginalBoundingBox() const;
	void setOriginalBoundingBox(const BoundingBox& newOriginalBoundingBox);

public:
	static std::string GetClassName() { return "CollisionComponent"; }

	virtual void toJson(nlohmann::json& outJson) const override;
	virtual void fromJson(const nlohmann::json& json) override;

	friend void to_json(nlohmann::json& outJson, const CollisionComponent& collision);
	friend void from_json(const nlohmann::json& json, CollisionComponent& outCollision);

	std::string getComponentTypeName() const override { return GetClassName(); }

private:
	Hull mGeometry;
	bool mBoundingBoxDirty = true;
	BoundingBox mBoundingBox = BoundingBox(Vector2D(0.0f, 0.0f), Vector2D(0.0f, 0.0f));
	BoundingBox mOriginalBoundingBox = BoundingBox(Vector2D(0.0f, 0.0f), Vector2D(0.0f, 0.0f));
};
