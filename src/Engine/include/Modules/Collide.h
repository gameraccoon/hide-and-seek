#pragma once

#include <stdio.h>

#include <Core/World.h>

#include <Components/CollisionComponent.generated.h>

namespace Collide
{
	bool doCollide(const CollisionComponent* collisionA, const Vector2D& locationA,
				   const CollisionComponent* collisionB, const Vector2D& locationB, Vector2D& outResist);

	void updateOriginalBoundingBox(CollisionComponent* collision);
}
