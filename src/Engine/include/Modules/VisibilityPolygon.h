#pragma once

#include <vector>

#include "Core/Vector2D.h"

class CollisionComponent;
class TransformComponent;

namespace VisibilityPolygon
{
	struct SimpleBorder
	{
		SimpleBorder(Vector2D a, Vector2D b) : a(a), b(b) {}
		Vector2D a;
		Vector2D b;
	};

	enum class PointSide
	{
		Left, // the point is free from the left (counter-clockwise) side
		Rignt, // the point is free from the right (clockwise) side
		InBetween // the point is connecting two borders that are facing the light source
	};

	struct AngledPoint
	{
		AngledPoint(Vector2D coords, float angle, PointSide side) : coords(coords), angle(angle), side(side) {}
		Vector2D coords;
		float angle;
		PointSide side;
	};

	// caches that can be reused
	struct Caches
	{
		std::vector<Vector2D> hullPoints;
		std::vector<SimpleBorder> borders;
		std::vector<AngledPoint> pointsToTrace;
		std::vector<Vector2D> resultPolygon;
	};

	float CalcClockwiseDirection(const Vector2D &a, const Vector2D &b);
	void CalculateVisibilityPolygon(Caches& outCaches, const std::vector<std::tuple<CollisionComponent*, TransformComponent*>>& components, const Vector2D& sourcePos, const Vector2D& polygonMaxSize);
}
