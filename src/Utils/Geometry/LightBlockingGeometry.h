#pragma once

#include <vector>

#include "Base/Types/TemplateAliases.h"

#include "ECS/Entity.h"

#include "GameData/Core/Hull.h"

#include "GameData/Core/Vector2D.h"

class WorldCell;
class CollisionComponent;
class TransformComponent;

namespace LightBlockingGeometry
{
	using CollisionGeometry = TupleVector<Entity, WorldCell*, CollisionComponent*, TransformComponent*>;

	struct CalculatedGeometry
	{
		CalculatedGeometry(const Hull& hull, const Vector2D& location)
			: hull(hull)
			, location(location)
		{}

		Hull hull;
		Vector2D location;
	};

	void CalculateLightGeometry(std::vector<CalculatedGeometry>& outGeometry, const CollisionGeometry& collisionGeometry);
}
