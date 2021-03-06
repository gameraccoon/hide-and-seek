#pragma once

#include <vector>

#include "Base/Types/TemplateAliases.h"

#include "ECS/Entity.h"

#include "GameData/Core/Hull.h"

#include "GameData/Core/Vector2D.h"
#include "GameData/Spatial/CellPos.h"

class WorldCell;
class CollisionComponent;
class TransformComponent;

namespace LightBlockingGeometry
{
	using CollisionGeometry = TupleVector<WorldCell*, CollisionComponent*, TransformComponent*>;
	using CalculatedGeometry = std::unordered_map<CellPos, std::vector<SimpleBorder>>;

	void CalculateLightGeometry(CalculatedGeometry& outGeometry, const CollisionGeometry& collisionGeometry);
}
