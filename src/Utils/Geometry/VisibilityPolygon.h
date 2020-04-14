#pragma once

#include <vector>

#include "Base/Types/TemplateAliases.h"

#include "GameData/Spatial/SpatialPoint.h"

class CollisionComponent;
class TransformComponent;

/**
 * Class that calculates visibility polygons
 *
 * Don't try to use one class in separate threads, instead use one instance per thread
 */
class VisibilityPolygonCalculator
{
public:
	VisibilityPolygonCalculator() = default;

	// returns reference to the calculated polygon
	void calculateVisibilityPolygon(std::vector<Vector2D>& outVisibilityPolygon, const TupleVector<CollisionComponent*, TransformComponent*>& components, SpatialPoint sourcePos, const Vector2D& polygonMaxSize);

	// copying objects of this class has a little sense and usually indicates a poor use
	VisibilityPolygonCalculator(VisibilityPolygonCalculator const&) = delete;
	VisibilityPolygonCalculator& operator=(VisibilityPolygonCalculator const&) = delete;

private:
	struct SimpleBorder
	{
		SimpleBorder(Vector2D a, Vector2D b) : a(a), b(b) {}
		Vector2D a;
		Vector2D b;
	};

	enum class PointSide
	{
		Left, // the point is free from the left (counter-clockwise) side
		Right, // the point is free from the right (clockwise) side
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
	};

private:
	static void AddPotentiallyVisiblePoint(bool isVisible, bool isPreviousVisible, const Vector2D& a, std::vector<AngledPoint>& pointsToTrace);
	static bool LessPointAngle(const AngledPoint& a, const AngledPoint& b);

private:
	Caches mCaches;
};
