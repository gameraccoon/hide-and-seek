#pragma once

#include <memory>

#include "Core/Component.h"

#include <Structures/Hull.h>
#include <Structures/BoundingBox.h>

#include <Core/Vector2D.h>

/**
 * Component that stores informtaon and logic about position and movement
 */
class CollisionComponent : public BaseComponent
{
public:
	using Ptr = std::shared_ptr<CollisionComponent>;
	using WeakPtr = std::weak_ptr<CollisionComponent>;

public:
	virtual ~CollisionComponent() override = default;

	const Hull& getGeometry() const;
	void setGeometry(const Hull& geometry);

	const BoundingBox& getBoundingBox() const;
	void setBoundingBox(const BoundingBox& boundingBox);

	bool isBoundingBoxDirty() const;

	const BoundingBox& getOriginalBoundingBox() const;
	void setOriginalBoundingBox(const BoundingBox& originalBoundingBox);

private:
	Hull mGeometry;
	bool mBoundingBoxDirty = true;
	BoundingBox mBoundingBox = BoundingBox(Vector2D(0.0f, 0.0f), Vector2D(0.0f, 0.0f));
	BoundingBox mOriginalBoundingBox = BoundingBox(Vector2D(0.0f, 0.0f), Vector2D(0.0f, 0.0f));
};


