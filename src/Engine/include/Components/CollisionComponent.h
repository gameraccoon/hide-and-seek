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
	virtual ~CollisionComponent();

	const Hull* getGeometry() const {return &mGeometry; }
	BoundingBox getBoundingBox() const {return BoundingBox(0.0f, 0.0f, 0.0f, 0.0f); }

private:
	Hull mGeometry;
};
