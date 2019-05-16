#include "Modules/VisibilityPolygon.h"

#include <algorithm>

#include "Components/TransformComponent.generated.h"
#include "Components/CollisionComponent.generated.h"

#include "Modules/RayTrace.h"
#include "Modules/Collide.h"

namespace VisibilityPolygon
{
	float CalcClockwiseDirection(const Vector2D &a, const Vector2D &b)
	{
		return a.x * (b.y - a.y) - a.y * (b.x - a.x);
	}

	static bool LessPointAngle(const AngledPoint& a, const AngledPoint& b)
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

	static void AddPotentiallyVisiblePoint(bool isVisible, bool isPreviousVisible, const Vector2D& a, std::vector<AngledPoint>& pointsToTrace)
	{
		if (isVisible)
		{
			if (isPreviousVisible)
			{
				pointsToTrace.push_back({a, a.rotation().getValue(), PointSide::InBetween});
			}
			else
			{
				pointsToTrace.push_back({a, a.rotation().getValue(), PointSide::Rignt});
			}
		}
		else
		{
			if (isPreviousVisible)
			{
				pointsToTrace.push_back({a, a.rotation().getValue(), PointSide::Left});
			}
		}
	}

	void CalculateVisibilityPolygon(Caches& outCaches, const std::vector<std::tuple<CollisionComponent*, TransformComponent*>>& components, const Vector2D& sourcePos, const Vector2D& polygonMaxSize)
	{
		outCaches.borders.clear();
		outCaches.borders.reserve(components.size() * 4 + 4);
		outCaches.pointsToTrace.clear();
		outCaches.pointsToTrace.reserve(components.size() * 4 + 4);

		// populate the borders of visible area
		float left = -polygonMaxSize.x * 0.5f;
		float right = polygonMaxSize.x * 0.5f;
		float top = -polygonMaxSize.y * 0.5f;
		float bottom = polygonMaxSize.y * 0.5f;
		Vector2D leftTop(left, top);
		Vector2D rightTop(right, top);
		Vector2D rightBottom(right, bottom);
		Vector2D leftBottom(left, bottom);
		outCaches.pointsToTrace.push_back({leftTop, leftTop.rotation().getValue(), PointSide::InBetween});
		outCaches.pointsToTrace.push_back({rightTop, rightTop.rotation().getValue(), PointSide::InBetween});
		outCaches.pointsToTrace.push_back({rightBottom, rightBottom.rotation().getValue(), PointSide::InBetween});
		outCaches.pointsToTrace.push_back({leftBottom, leftBottom.rotation().getValue(), PointSide::InBetween});
		outCaches.borders.push_back({leftTop, rightTop});
		outCaches.borders.push_back({rightTop, rightBottom});
		outCaches.borders.push_back({rightBottom, leftBottom});
		outCaches.borders.push_back({leftBottom, leftTop});

		// find borders that facing the light source and their points
		for (const auto& [collision, transform] : components)
		{
			Vector2D shift = transform->getLocation() - sourcePos;
			const Hull& hull = collision->getGeometry();
			bool isPreviousVisible = false;
			bool isNotFirst = false;
			bool isFirstVisible = false;
			// copy to be able to modify
			outCaches.hullPoints = hull.points;
			size_t hullSize = hull.points.size();
			for (size_t i = 0; i < hullSize; ++i)
			{
				size_t j = (i < hullSize - 1) ? (i+1) : 0;
				Vector2D a = outCaches.hullPoints[i] + shift;
				Vector2D b = outCaches.hullPoints[j] + shift;
				// keep only borders inside the draw area and facing the light source
				bool isVisible = ((a.x > left && a.x < right && a.y > top && a.y < bottom)
								  || (b.x > left && b.x < right && b.y > top && b.y < bottom)
								  ) && CalcClockwiseDirection(a, b) < 0.0f;

				if (isVisible)
				{
					if (NormalizePoint(a, b, left, right, top, bottom))
					{
						outCaches.hullPoints[i] = a - shift;
					}
					if (NormalizePoint(b, a, left, right, top, bottom))
					{
						outCaches.hullPoints[j] = b - shift;
					}
					outCaches.borders.push_back({b, a});
				}

				if (isNotFirst)
				{
					AddPotentiallyVisiblePoint(isVisible, isPreviousVisible, a, outCaches.pointsToTrace);
				}
				else
				{
					isFirstVisible = isVisible;
					isNotFirst = true;
				}
				isPreviousVisible = isVisible;
			}

			if (outCaches.hullPoints.size() > 1)
			{
				Vector2D a = outCaches.hullPoints[0] + shift;
				AddPotentiallyVisiblePoint(isFirstVisible, isPreviousVisible, a, outCaches.pointsToTrace);
			}
		}

		if (outCaches.borders.size() == 0)
		{
			return;
		}

		// sort points that we can iterate over them in clockwise order
		std::sort(outCaches.pointsToTrace.begin(), outCaches.pointsToTrace.end(), LessPointAngle);

		// fix a corner case with two or more points have exact same angles
		for (size_t i = 1; i < outCaches.pointsToTrace.size();)
		{
			AngledPoint& item = outCaches.pointsToTrace[i];
			AngledPoint& previousItem = outCaches.pointsToTrace[i - 1];
			// the problem is present only if the angles are exactly equal
			if (item.angle != previousItem.angle)
			{
				++i;
			}
			else
			{
				if (item.coords.qSize() > previousItem.coords.qSize())
				{
					outCaches.pointsToTrace.erase(outCaches.pointsToTrace.begin() + static_cast<int>(i));
				}
				else
				{
					outCaches.pointsToTrace.erase(outCaches.pointsToTrace.begin() + static_cast<int>(i - 1));
				}
			}
		}

		// calculate visibility polygon
		outCaches.resultPolygon.clear();
		Vector2D nearestPoint;
		for (AngledPoint& point : outCaches.pointsToTrace)
		{
			nearestPoint = point.coords;
			bool notFound = true;
			bool needToSkip = false;
			for (SimpleBorder& border : outCaches.borders)
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
					outCaches.resultPolygon.push_back(nearestPoint);
					outCaches.resultPolygon.push_back(point.coords);
				}
				else if (point.side == PointSide::Rignt)
				{
					outCaches.resultPolygon.push_back(point.coords);
					outCaches.resultPolygon.push_back(nearestPoint);
				}
			}
			else
			{
				outCaches.resultPolygon.push_back(point.coords);
			}
		}
	}
}
