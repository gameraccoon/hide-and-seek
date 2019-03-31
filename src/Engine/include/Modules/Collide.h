#pragma once

#include <stdio.h>

#include <Core/World.h>

#include <Components/CollisionComponent.h>
#include <Components/TransformComponent.h>

namespace Collide
{
	bool doCollide(const CollisionComponent::Ptr& collisionA, const TransformComponent::Ptr& transformA,
				   const CollisionComponent::Ptr& collisionB, const TransformComponent::Ptr& transformB);
}
