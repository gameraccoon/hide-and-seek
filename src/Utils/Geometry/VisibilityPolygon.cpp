#include "Base/precomp.h"

#include "Utils/Geometry/VisibilityPolygon.h"

#include <algorithm>

#include "Base/Math/Float.h"

#include "GameData/Components/TransformComponent.generated.h"
#include "GameData/Components/CollisionComponent.generated.h"
#include "GameData/Spatial/SpatialWorldData.h"

#include "Utils/Geometry/RayTrace.h"
#include "Utils/Geometry/Collide.h"

static float CalcClockwiseDirection(const Vector2D& a, const Vector2D& b)
{
	return a.x * (b.y - a.y) - a.y * (b.x - a.x);
}

bool VisibilityPolygonCalculator::LessPointAngle(const AngledPoint& a, const AngledPoint& b)
{
	return a.angle < b.angle;
}

static bool NormalizePoint(Vector2D& point, const Vector2D& pair, float left, float right, float top, float bottom)
{
	if (point.x < left)
	{
		point = pair + abs((left - pair.x)/(point.x - pair.x)) * (point-pair);
		return true;
	}
	else if (point.x > right)
	{
		point = pair + abs((pair.x - right)/(pair.x - point.x)) * (point-pair);
		return true;
	}

	if (point.y < top)
	{
		point = pair + abs((top - pair.y)/(point.y - pair.y)) * (point-pair);
		return true;
	}
	else if (point.y > bottom)
	{
		point = pair + abs((pair.y - bottom)/(pair.y - point.y)) * (point-pair);
		return true;
	}
	return false;
}

void VisibilityPolygonCalculator::AddPotentiallyVisiblePoint(bool isVisible, bool isPreviousVisible, const Vector2D& a, std::vector<AngledPoint>& pointsToTrace)
{
	if (isVisible)
	{
		if (isPreviousVisible)
		{
			pointsToTrace.emplace_back(a, a.rotation().getValue(), PointSide::InBetween);
		}
		else
		{
			pointsToTrace.emplace_back(a, a.rotation().getValue(), PointSide::Right);
		}
	}
	else
	{
		if (isPreviousVisible)
		{
			pointsToTrace.emplace_back(a, a.rotation().getValue(), PointSide::Left);
		}
	}
}

void VisibilityPolygonCalculator::calculateVisibilityPolygon(std::vector<Vector2D>& outVisibilityPolygon, const TupleVector<CollisionComponent*, TransformComponent*>& components, Vector2D sourcePos, Vector2D polygonMaxSize)
{
	outVisibilityPolygon.clear();

	mCaches.borders.clear();
	mCaches.borders.reserve(components.size() * 4 + 4);
	mCaches.pointsToTrace.clear();
	mCaches.pointsToTrace.reserve(components.size() * 4 + 4);

	// populate the borders of visible area
	float left = -polygonMaxSize.x * 0.5f;
	float right = polygonMaxSize.x * 0.5f;
	float top = -polygonMaxSize.y * 0.5f;
	float bottom = polygonMaxSize.y * 0.5f;
	Vector2D leftTop(left, top);
	Vector2D rightTop(right, top);
	Vector2D rightBottom(right, bottom);
	Vector2D leftBottom(left, bottom);
	mCaches.pointsToTrace.emplace_back(leftTop, leftTop.rotation().getValue(), PointSide::InBetween);
	mCaches.pointsToTrace.emplace_back(rightTop, rightTop.rotation().getValue(), PointSide::InBetween);
	mCaches.pointsToTrace.emplace_back(rightBottom, rightBottom.rotation().getValue(), PointSide::InBetween);
	mCaches.pointsToTrace.emplace_back(leftBottom, leftBottom.rotation().getValue(), PointSide::InBetween);
	mCaches.borders.emplace_back(leftTop, rightTop);
	mCaches.borders.emplace_back(rightTop, rightBottom);
	mCaches.borders.emplace_back(rightBottom, leftBottom);
	mCaches.borders.emplace_back(leftBottom, leftTop);

	// find borders that facing the light source and their points
	for (const auto& [collision, transform] : components)
	{
		Vector2D shift = transform->getLocation() - sourcePos;
		const Hull& hull = collision->getGeometry();
		bool isPreviousVisible = false;
		bool isNotFirst = false;
		bool isFirstVisible = false;
		// copy to be able to modify
		mCaches.hullPoints.resize(hull.points.size());
		std::copy(hull.points.begin(), hull.points.end(), mCaches.hullPoints.begin());
		//outCaches.hullPoints = hull.points;
		size_t hullSize = hull.points.size();
		for (size_t i = 0; i < hullSize; ++i)
		{
			size_t j = (i < hullSize - 1) ? (i+1) : 0;
			Vector2D a = mCaches.hullPoints[i] + shift;
			Vector2D b = mCaches.hullPoints[j] + shift;
			// keep only borders inside the draw area and facing the light source
			bool isVisible = ((a.x > left && a.x < right && a.y > top && a.y < bottom)
							  || (b.x > left && b.x < right && b.y > top && b.y < bottom)
							  ) && CalcClockwiseDirection(a, b) < 0.0f;

			if (isVisible)
			{
				if (NormalizePoint(a, b, left, right, top, bottom))
				{
					mCaches.hullPoints[i] = a - shift;
				}
				if (NormalizePoint(b, a, left, right, top, bottom))
				{
					mCaches.hullPoints[j] = b - shift;
				}
				mCaches.borders.emplace_back(b, a);
			}

			if (isNotFirst)
			{
				AddPotentiallyVisiblePoint(isVisible, isPreviousVisible, a, mCaches.pointsToTrace);
			}
			else
			{
				isFirstVisible = isVisible;
				isNotFirst = true;
			}
			isPreviousVisible = isVisible;
		}

		if (mCaches.hullPoints.size() > 1)
		{
			Vector2D a = mCaches.hullPoints[0] + shift;
			AddPotentiallyVisiblePoint(isFirstVisible, isPreviousVisible, a, mCaches.pointsToTrace);
		}
	}

	if (mCaches.borders.size() == 0)
	{
		return;
	}

	// sort points that we can iterate over them in clockwise order
	std::sort(mCaches.pointsToTrace.begin(), mCaches.pointsToTrace.end(), LessPointAngle);

	// fix a corner case with two or more points have exact same angles
	for (size_t i = 1; i < mCaches.pointsToTrace.size();)
	{
		const AngledPoint& item = mCaches.pointsToTrace[i];
		const AngledPoint& previousItem = mCaches.pointsToTrace[i - 1];
		// the problem is present only if the angles are exactly equal
		if (item.angle != previousItem.angle)
		{
			++i;
		}
		else
		{
			if (item.coords.qSize() > previousItem.coords.qSize())
			{
				if (previousItem.side == PointSide::InBetween)
				{
					mCaches.pointsToTrace.erase(mCaches.pointsToTrace.begin() + static_cast<int>(i));
				}
				else
				{
					if (previousItem.side == PointSide::Left)
					{
						std::iter_swap(mCaches.pointsToTrace.begin() + static_cast<int>(i), mCaches.pointsToTrace.begin() + static_cast<int>(i - 1));
					}
					++i;
				}
			}
			else
			{
				if (item.side == PointSide::InBetween)
				{
					mCaches.pointsToTrace.erase(mCaches.pointsToTrace.begin() + static_cast<int>(i - 1));
				}
				else
				{
					if (previousItem.side == PointSide::Right)
					{
						std::iter_swap(mCaches.pointsToTrace.begin() + static_cast<int>(i), mCaches.pointsToTrace.begin() + static_cast<int>(i - 1));
					}
					++i;
				}
			}
		}
	}

	// calculate visibility polygon
	Vector2D nearestPoint;
	for (AngledPoint& point : mCaches.pointsToTrace)
	{
		nearestPoint = point.coords;
		bool notFound = true;
		bool needToSkip = false;
		for (SimpleBorder& border : mCaches.borders)
		{
			if (CalcClockwiseDirection(border.a, point.coords) > 0.0f
				&&
				CalcClockwiseDirection(point.coords, border.b) > 0.0f)
			{
				if (Collide::SignedArea(border.a, border.b, point.coords) < 0.0f)
				{
					// this point is hidden behind some obstacle
					needToSkip = true;
					break;
				}

				if (point.side == PointSide::InBetween)
				{
					// don't trace through in-between points
					continue;
				}

				Vector2D intersectPoint = RayTrace::GetPointIntersect2Lines(border.a, border.b, ZERO_VECTOR, point.coords);
				if (notFound || intersectPoint.qSize() < nearestPoint.qSize())
				{
					nearestPoint = intersectPoint;
					notFound = false;
				}
			}
		}

		if (needToSkip)
		{
			continue;
		}

		// add point (and optionally its casted shadow) to the polygon
		if (nearestPoint != point.coords)
		{
			if (point.side == PointSide::Left)
			{
				outVisibilityPolygon.push_back(nearestPoint);
				outVisibilityPolygon.push_back(point.coords);
			}
			else if (point.side == PointSide::Right)
			{
				outVisibilityPolygon.push_back(point.coords);
				outVisibilityPolygon.push_back(nearestPoint);
			}
		}
		else
		{
			outVisibilityPolygon.push_back(point.coords);
		}
	}
}
