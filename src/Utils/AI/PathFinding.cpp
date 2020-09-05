#include "Base/precomp.h"

#include <unordered_map>
#include <list>

#include "Utils/AI/PathFinding.h"

#include "GameData/AI/NavMesh.h"
#include "GameData/Core/BoundingBox.h"

#include "Utils/Geometry/Collide.h"

namespace PathFinding
{
	static constexpr size_t InvalidPolygon = std::numeric_limits<size_t>::max();

	bool IsPointInsideConvexHull(Vector2D point, const std::vector<Vector2D>& hull)
	{
		FOR_EACH_BORDER(hull.size(),
		{
			if (Collide::SignedArea(point, hull[i], hull[j]) < 0.0f)
			{
				return false;
			}
		});

		return true;
	}

	static size_t FindPolygonForPoint(Vector2D point, const NavMesh& navMesh)
	{
		Vector2D cellPointFloat;
		cellPointFloat = (point - navMesh.geometry.navMeshStart) / navMesh.spatialHash.cellSize;
		IntVector2D cellPoint(static_cast<int>(cellPointFloat.x), static_cast<int>(cellPointFloat.y));
		if (cellPoint.x < 0 || cellPoint.x >= navMesh.spatialHash.hashSize.x
			||
			cellPoint.y < 0 || cellPoint.y >= navMesh.spatialHash.hashSize.y)
		{
			return InvalidPolygon;
		}

		const std::vector<size_t>& polygons = navMesh.spatialHash.polygonsHash[cellPoint.x + cellPoint.y * navMesh.spatialHash.hashSize.x];
		std::vector<Vector2D> polygonPoints(navMesh.geometry.vertsPerPoly);
		for (size_t polygon : polygons)
		{
			size_t polyShift = polygon * navMesh.geometry.vertsPerPoly;
			for (size_t i = 0; i < navMesh.geometry.vertsPerPoly; ++i){
				polygonPoints[i] = navMesh.geometry.vertices[navMesh.geometry.indexes[polyShift + i]];
			}

			if (IsPointInsideConvexHull(point, polygonPoints))
			{
				return polygon;
			}
		}

		return InvalidPolygon;
	}

	struct LineSegmentToNeighborIntersection
	{
		NavMesh::InnerLinks::LinkData link;
		Vector2D intersectionPoint;
	};

	static_assert(std::is_trivially_copyable<LineSegmentToNeighborIntersection>(), "LineSegmentToNeighborIntersection should be trivially copyable");
	static_assert(std::is_trivially_constructible<LineSegmentToNeighborIntersection>(), "LineSegmentToNeighborIntersection should be trivially constructible");

	static LineSegmentToNeighborIntersection FindLineSegmentToNeighborIntersection(const NavMesh& navMesh, Vector2D start, Vector2D finish, size_t polygon, size_t ignoredNeighbor)
	{
		LineSegmentToNeighborIntersection result;
		result.link.neighbor = InvalidPolygon;
		float bestIntersectionQDistance = 0.0f;

		for (const NavMesh::InnerLinks::LinkData& link : navMesh.links.links[polygon])
		{
			if (link.neighbor != ignoredNeighbor)
			{
				Vector2D vert1 = navMesh.geometry.vertices[link.borderPoint1];
				Vector2D vert2 = navMesh.geometry.vertices[link.borderPoint2];

				bool areIntersect = Collide::AreLinesIntersect(start, finish, vert1, vert2);

				if (areIntersect)
				{
					Vector2D intersectionPoint = Collide::GetPointIntersect2Lines(start, finish, vert1, vert2);
					float intersectionQDistance = (intersectionPoint - finish).qSize();
					if (result.link.neighbor == InvalidPolygon
						// choose the variant closer to the finish point
						|| intersectionQDistance < bestIntersectionQDistance
						)
					{
						result.link = link;
						result.intersectionPoint = intersectionPoint;
						bestIntersectionQDistance = intersectionQDistance;
					}
				}
			}
		}

		return result;
	}

	static std::vector<size_t> GetIntersectedNeighbors(const NavMesh& navMesh, Vector2D start, Vector2D finish, size_t polygon, size_t ignoredNeighbor)
	{
		std::vector<size_t> result;

		for (const NavMesh::InnerLinks::LinkData& link : navMesh.links.links[polygon])
		{
			if (link.neighbor != ignoredNeighbor)
			{
				Vector2D vert1 = navMesh.geometry.vertices[link.borderPoint1];
				Vector2D vert2 = navMesh.geometry.vertices[link.borderPoint2];

				bool areIntersect = Collide::AreLinesIntersect(start, finish, vert1, vert2);

				if (areIntersect)
				{
					result.push_back(link.neighbor);
				}
			}
		}

		return result;
	}

	struct PointScores
	{
		float g;
		float h;
		float f;
	};

	struct PathPoint
	{
		size_t polygon;
		Vector2D pos;
		PointScores scores;
		size_t previous;
	};

	static_assert(std::is_trivially_copyable<PathPoint>(), "PathPoint should be trivially copyable");
	static_assert(std::is_trivially_constructible<PathPoint>(), "PathPoint should be trivially constructible");

	using OpenListType = std::multimap<float, PathPoint>;
	using OpenMapType = std::unordered_map<size_t, OpenListType::iterator>;
	using ClosedListType = std::unordered_map<size_t, PathPoint>;

	static void AddToOpenListIfBetter(OpenListType& openList, OpenMapType& openMap, const PathPoint& point)
	{
		auto [mapIterator, isInserted] = openMap.try_emplace(point.polygon, openList.end());
		// if the node is new
		if (isInserted)
		{
			// add the new node to the openList and save its iterator to openMap
			mapIterator->second = openList.emplace(point.scores.f, point);
		}
		else
		{
			// if the node is better than the previous
			if (point.scores.f < mapIterator->second->second.scores.f)
			{
				// remove the old node
				openList.erase(mapIterator->second);
				// add the new node to the openList and save its iterator to openMap
				mapIterator->second = openList.emplace(point.scores.f, point);
			}
		}
		AssertFatal(openList.size() == openMap.size(), "openList and openMap have diverged");
	}

	static bool AddToClosedListIfBetter(ClosedListType& closedList, const PathPoint& point)
	{
		auto [mapIterator, isInserted] = closedList.try_emplace(point.polygon, point);

		if (!isInserted)
		{
			if (point.scores.f < mapIterator->second.scores.f)
			{
				mapIterator->second = point;
			}
			else
			{
				return false;
			}
		}
		return true;
	}

	static PathPoint PopBestFromOpenList(OpenListType& openList, OpenMapType& openMap)
	{
		PathPoint result = std::move(openList.begin()->second);
		openList.erase(openList.begin());
		openMap.erase(result.polygon);
		AssertFatal(openList.size() == openMap.size(), "openList and openMap have diverged");
		return result;
	}

	static PointScores CalculatePointScores(Vector2D pos, float previousG, Vector2D previousPos, Vector2D target)
	{
		PointScores result;
		result.g = previousG + (pos - previousPos).size();
		result.h = (target - pos).size();
		result.f = result.g + result.h;
		return result;
	}

	static bool CanBuildStraightPathRecursive(const NavMesh& navMesh, Vector2D point1, Vector2D point2, size_t currentPolygon, size_t lastPolygon, size_t previousPolygon)
	{
		std::vector<size_t> intersections = GetIntersectedNeighbors(navMesh, point1, point2, currentPolygon, previousPolygon);

		for (size_t intersection : intersections)
		{
			if (intersection == lastPolygon)
			{
				return true;
			}

			if (CanBuildStraightPathRecursive(navMesh, point1, point2, intersection, lastPolygon, currentPolygon))
			{
				return true;
			}
		}

		return false;
	}

	static bool CanBuildStraightPath(const NavMesh& navMesh, const PathPoint& point1, const PathPoint& point2)
	{
		Vector2D smallAdvancement = (point2.pos - point1.pos).unit() * 0.1f;
		Vector2D secondPoint = point2.pos + smallAdvancement;
		return CanBuildStraightPathRecursive(navMesh, point1.pos, secondPoint, point1.polygon, point2.polygon, InvalidPolygon);
	}

	static void OptimizePath(std::vector<PathPoint>& inOutPath, const NavMesh& navMesh)
	{
		if (!inOutPath.empty())
		{
			for (size_t i = inOutPath.size() - 1; i >= 2; --i)
			{
				if (CanBuildStraightPath(navMesh, inOutPath[i], inOutPath[i - 2]))
				{
					inOutPath.erase(inOutPath.begin() + (i - 1));
				}
			}
		}
	}

	void FindPath(std::vector<Vector2D>& outPath, const NavMesh& navMesh, Vector2D start, Vector2D finish)
	{
		outPath.clear();

		size_t startPolygon = FindPolygonForPoint(start, navMesh);
		size_t finishPolygon = FindPolygonForPoint(finish, navMesh);

		if (startPolygon == InvalidPolygon || finishPolygon == InvalidPolygon)
		{
			outPath.push_back(start);
			outPath.push_back(finish);
			return;
		}

		// open list sorted by the f value
		OpenListType openList;
		OpenMapType openMap;
		ClosedListType closedList;

		{
			PathPoint firstPoint;
			firstPoint.polygon = finishPolygon;
			firstPoint.previous = InvalidPolygon;
			// we are moving from finish to start, to then rewind the path
			firstPoint.pos = finish;
			firstPoint.scores.g = 0.0f;
			firstPoint.scores.h = (start - finish).size();
			firstPoint.scores.f = firstPoint.scores.g + firstPoint.scores.h;
			AddToOpenListIfBetter(openList, openMap, firstPoint);
		}

		// A* improved with first checks by raytracing

		unsigned int stepLimit = 100u;
		unsigned int step = 0u;
		PathPoint currentPoint;
		while (!openList.empty() && step < stepLimit)
		{
			++step;

			currentPoint = PopBestFromOpenList(openList, openMap);
			bool isBetter = AddToClosedListIfBetter(closedList, currentPoint);

			if (currentPoint.polygon == startPolygon)
			{
				break;
			}

			if (!isBetter)
			{
				continue;
			}

			// find raycast neighbor (best possible neighbor from this point)
			LineSegmentToNeighborIntersection rayIntersection = FindLineSegmentToNeighborIntersection(navMesh, currentPoint.pos, start, currentPoint.polygon, currentPoint.previous);
			size_t bestNeighborID = rayIntersection.link.neighbor;

			if (bestNeighborID != InvalidPolygon)
			{
				PathPoint point;
				point.polygon = bestNeighborID;
				point.previous = currentPoint.polygon;
				point.pos = rayIntersection.intersectionPoint;
				point.scores = CalculatePointScores(point.pos, currentPoint.scores.g, currentPoint.pos, start);

				AddToOpenListIfBetter(openList, openMap, point);
			}

			for (const NavMesh::InnerLinks::LinkData& link : navMesh.links.links[currentPoint.polygon])
			{
				// we already added bestNeighborID node to the open list
				// also skip the previous point where we came from
				if (link.neighbor == bestNeighborID || link.neighbor == currentPoint.previous)
				{
					continue;
				}

				// select better border point
				Vector2D borderPointA = navMesh.geometry.vertices[link.borderPoint1];
				Vector2D borderPointB = navMesh.geometry.vertices[link.borderPoint2];

				PointScores scoresA = CalculatePointScores(borderPointA, currentPoint.scores.g, currentPoint.pos, start);
				PointScores scoresB = CalculatePointScores(borderPointB, currentPoint.scores.g, currentPoint.pos, start);

				PathPoint point;
				point.polygon = link.neighbor;
				point.previous = currentPoint.polygon;

				if (scoresA.f < scoresB.f)
				{
					point.pos = borderPointA;
					point.scores = scoresA;
				}
				else
				{
					point.pos = borderPointB;
					point.scores = scoresB;
				}

				AddToOpenListIfBetter(openList, openMap, point);
			}
		}

		std::vector<PathPoint> bestPath;
		{
			// add start point (it is not in the closed list)
			{
				PathPoint point;
				point.polygon = startPolygon;
				point.pos = start;
				bestPath.push_back(std::move(point));
			}

			// unwind the path
			size_t nextPolygon = currentPoint.polygon;
			while (nextPolygon != InvalidPolygon)
			{
				bestPath.push_back(closedList[nextPolygon]);
				nextPolygon = bestPath.back().previous;
			}
		}

		OptimizePath(bestPath, navMesh);

		for (const PathPoint& point : bestPath)
		{
			outPath.push_back(point.pos);
		}
	}
} // namespace PathFinding
