#include "Base/precomp.h"

#include "Utils/Geometry/ShapeOperations.h"

#include <unordered_map>
#include <unordered_set>

#include "Utils/Geometry/Collide.h"

namespace ShapeOperations
{
	struct BorderPoint
	{
		enum class Direction
		{
			Previous,
			Next,
			None
		};

		explicit BorderPoint(Vector2D pos, Direction direction = Direction::None)
			: pos(pos)
			, cutDirection(direction)
		{}

		Vector2D pos;
		Direction cutDirection;
	};

	struct BorderIntersection
	{
		BorderIntersection(size_t intersectionPoint, bool isFirstOutside)
			: intersectionPoint(intersectionPoint)
			, isFirstOutside(isFirstOutside)
		{}

		size_t intersectionPoint;
		bool isFirstOutside;
	};

	std::vector<SimpleBorder> GetUnion(const std::vector<SimpleBorder>& shape1, const std::vector<SimpleBorder>& shape2)
	{
		const size_t shape1BordersCount = shape1.size();
		std::vector<Vector2D> intersectionPoints;
		std::unordered_set<Vector2D> intersectionPointsSet;
		std::unordered_map<size_t, std::vector<BorderIntersection>> borderIntersections;

		// find all intersections
		for (size_t i = 0; i < shape1.size(); ++i)
		{
			const SimpleBorder& border = shape1[i];
			// create items even for the borders without intersections
			std::vector<BorderIntersection>& intersections1 = borderIntersections[i];
			for (size_t j = 0; j < shape2.size(); ++j)
			{
				const SimpleBorder& otherBorder = shape2[j];
				if (!Collide::AreLinesIntersect(border.a, border.b, otherBorder.a, otherBorder.b))
				{
					continue;
				}
				Vector2D intersectionPoint = Collide::GetPointIntersect2Lines(border.a, border.b, otherBorder.a, otherBorder.b);

				intersectionPointsSet.insert(intersectionPoint);
				intersectionPoints.push_back(intersectionPoint);
				size_t pointIdx = intersectionPoints.size() - 1;
				// we can calculate wether a border part inside the resulting figure or outside from calculating
				// on which point of another border lies it point, we can do that by determining whether any three points
				// are in clockwise or counterclockwise order
				bool isClockwiseRotation = Collide::SignedArea(border.a, otherBorder.b, intersectionPoint) > 0;
				intersections1.emplace_back(pointIdx, !isClockwiseRotation);
				borderIntersections[shape1BordersCount + j].emplace_back(pointIdx, isClockwiseRotation);
			}
		}

		{
			// create empty items for the borders without intersections
			for (size_t j = 0; j < shape2.size(); ++j)
			{
				borderIntersections.try_emplace(shape1BordersCount + j);
			}
		}

		std::vector<SimpleBorder> fracturedBorders;
		std::unordered_map<Vector2D, std::vector<size_t>> borderConnections;

		// split borders into smaller parts
		for (const auto& [borderIndex, intersections] : borderIntersections)
		{
			const SimpleBorder& border = (borderIndex < shape1BordersCount) ? shape1[borderIndex] : shape2[borderIndex - shape1BordersCount];

			std::vector<BorderPoint> borderPoints({
				BorderPoint(border.a),
				BorderPoint(border.b)
			});
			std::vector<float> borderPointFractions({0.0f, 1.0f});
			for (auto [intersectionIdx, isPreviousOutside] : intersections)
			{
				Vector2D intersectionPoint = intersectionPoints[intersectionIdx];
				float intersectionFraction = Vector2D::InvLerp(border.a, border.b, intersectionPoint);
				// ToDo: replace binary search with linear (it's likely to have a very small amount of intersections)
				auto it = std::lower_bound(borderPointFractions.begin(), borderPointFractions.end(), intersectionFraction);
				size_t newPosition = std::distance(borderPointFractions.begin(), it);
				borderPointFractions.insert(it, intersectionFraction);
				BorderPoint::Direction cutDirection = isPreviousOutside ? BorderPoint::Direction::Next : BorderPoint::Direction::Previous;
				borderPoints.emplace(borderPoints.begin() + newPosition, intersectionPoint, cutDirection);
			}

			for (size_t i = 1; i < borderPoints.size(); ++i)
			{
				fracturedBorders.emplace_back(borderPoints[i-1].pos, borderPoints[i].pos);

				if (borderPoints[i-1].cutDirection != BorderPoint::Direction::Next
					&& borderPoints[i].cutDirection != BorderPoint::Direction::Previous)
				{
					borderConnections[fracturedBorders.back().a].push_back(fracturedBorders.size() - 1);
				}
			}
		}

		// find all chains of borders that should be eliminated
		std::vector<size_t> bordersToRemove;
		for (Vector2D intersectionPoint : intersectionPoints)
		{
			const std::vector<size_t>& connections = borderConnections[intersectionPoint];
			for (size_t startBorderIdx : connections)
			{
				if (fracturedBorders[startBorderIdx].a == intersectionPoint)
				{
					size_t borderIdx = startBorderIdx;
					while(true) // exit in the middle
					{
						bordersToRemove.push_back(borderIdx);
						Vector2D nextPoint = fracturedBorders[borderIdx].b;
						const std::vector<size_t>& nextConnections = borderConnections[nextPoint];
						size_t nextConnectionsCount = nextConnections.size();

						if (nextConnectionsCount != 1)
						{
							break;
						}

						if (intersectionPointsSet.find(nextPoint) != intersectionPointsSet.end())
						{
							break;
						}

						borderIdx = nextConnections[0];
					}
				}
			}
		}

		std::sort(bordersToRemove.begin(), bordersToRemove.end(), std::greater());
		bordersToRemove.erase(std::unique(bordersToRemove.begin(), bordersToRemove.end()), bordersToRemove.end());

		for (size_t index : bordersToRemove)
		{
			fracturedBorders.erase(fracturedBorders.begin() + index);
		}

		return fracturedBorders;
	}
}
