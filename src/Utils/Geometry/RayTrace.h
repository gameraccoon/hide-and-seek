#pragma once

#include "GameData/Core/Vector2D.h"
#include "GameData/Spatial/SpatialEntity.h"

class World;
struct Vector2D;
class BoundingBox;

namespace RayTrace
{
	struct TraceResult {
		bool hasHit = false;
		SpatialEntity hitEntity;
		Vector2D hitPoint;
		Vector2D hitBorderNormal;
	};

	bool AreLinesIntersect(const Vector2D& A1, const Vector2D& A2, const Vector2D& B1, const Vector2D& B2);
	bool IsLineIntersectAABB(const BoundingBox& box, const Vector2D& first, const Vector2D& last);
	bool FastTrace(World& world, const Vector2D& startPoint, const Vector2D& endPoint);
	TraceResult Trace(World& world, Vector2D startPoint, Vector2D endPoint);
	Vector2D GetPointIntersect2Lines(const Vector2D& A1, const Vector2D& A2, const Vector2D& B1, const Vector2D& B2);
}
