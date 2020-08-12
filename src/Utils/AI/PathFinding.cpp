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

/*	std::pair<size_t, size_t> GetNeighborBorder(const NavMesh::InnerLinks& links, size_t current, size_t neighbor)
	{
		for (const NavMesh::InnerLinks::LinkData& link : links.links[current])
		{
			if (link.neighbor == neighbor)
			{
				return link.border;
			}
		}

		return std::make_pair(0u, 0u);
	}

	struct LineSegmentToPolygonIntersection
	{
		size_t borderPointA;
		size_t borderPointB;
		Vector2D point;
	};

	static LineSegmentToPolygonIntersection FindLineSegmentToPolygonIntersection(const NavMesh::Geometry& geometry, Vector2D start, Vector2D finish, size_t polygon, const std::pair<size_t, size_t>& ignoredBorder)
	{
		LineSegmentToPolygonIntersection result;

		size_t polygonShift = polygon * geometry.vertsPerPoly;
		FOR_EACH_BORDER(geometry.vertsPerPoly,
		{
			if (polygonShift + i != ignoredBorder.first || polygonShift + j != ignoredBorder.second)
			{
				Vector2D vert1 = geometry.vertices[geometry.indexes[polygonShift + i]];
				Vector2D vert2 = geometry.vertices[geometry.indexes[polygonShift + j]];

				bool areIntersect = Collide::AreLinesIntersect(start, finish, vert1, vert2);

				if (areIntersect)
				{
					result.borderPointA = geometry.indexes[polygonShift + i];
					result.borderPointB = geometry.indexes[polygonShift + j];
					result.point = Collide::GetPointIntersect2Lines(start, finish, vert1, vert2);
					return result;
				}
			}
		});

		return result;
	}

	static size_t FindNeighborWithBorder(const NavMesh::InnerLinks& links, size_t currentPolygon, size_t borderPointA, size_t borderPointB)
	{
		for (const NavMesh::InnerLinks::LinkData& link : links.links[currentPolygon])
		{
			if (link.border.first == borderPointA && link.border.second == borderPointB)
			{
				return link.neighbor;
			}
		}

		return InvalidPolygon;
	}*/

	struct PathPoint
	{
		size_t polygon;
		Vector2D pos;
		float g;
		float h;
		float f;
		size_t previous;
	};

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
			mapIterator->second = openList.emplace(point.f, point);
		}
		else
		{
			// if the node is better than the previous
			if (point.f < mapIterator->second->second.f)
			{
				// remove the old node
				openList.erase(mapIterator->second);
				// add the new node to the openList and save its iterator to openMap
				mapIterator->second = openList.emplace(point.f, point);
			}
		}
		AssertFatal(openList.size() == openMap.size(), "openList and openMap have diverged");
	}

	static void AddToClosedListIfBetter(ClosedListType& closedList, const PathPoint& point)
	{
		auto [mapIterator, isInserted] = closedList.emplace(point.polygon, point);

		if (!isInserted)
		{
			if (point.f < mapIterator->second.f)
			{
				mapIterator->second = point;
			}
		}
	}

	static PathPoint PopBestFromOpenList(OpenListType& openList, OpenMapType& openMap)
	{
		PathPoint result = std::move(openList.begin()->second);
		openList.erase(openList.begin());
		openMap.erase(result.polygon);
		AssertFatal(openList.size() == openMap.size(), "openList and openMap have diverged");
		return result;
	}

	static void CalculatePointData(PathPoint& point, float previousG, Vector2D previousPos, Vector2D target)
	{
		point.g = previousG + (point.pos - previousPos).size();
		point.h = (target - point.pos).size();
		point.f = point.g + point.h;
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
			firstPoint.g = 0.0f;
			firstPoint.h = (start - finish).size();
			firstPoint.f = firstPoint.g + firstPoint.h;
			AddToOpenListIfBetter(openList, openMap, firstPoint);
		}

		unsigned int stepLimit = 100u;
		unsigned int step = 0u;
		PathPoint currentPoint;
		while (!openList.empty() && step < stepLimit)
		{
			++step;

			currentPoint = PopBestFromOpenList(openList, openMap);
			AddToClosedListIfBetter(closedList, currentPoint);

			if (currentPoint.polygon == startPolygon)
			{
				break;
			}

			// find raycast neighbor (best possible neighbor from this point)
/*			std::pair<size_t, size_t> ignoredBorder = GetNeighborBorder(navMesh.links, currentPoint.polygon, currentPoint.previous);
			LineSegmentToPolygonIntersection rayIntersection = FindLineSegmentToPolygonIntersection(navMesh.geometry, currentPoint.pos, start, currentPoint.polygon, ignoredBorder);
			size_t bestNeighborID = FindNeighborWithBorder(navMesh.links, currentPoint.polygon, rayIntersection.borderPointA, rayIntersection.borderPointB);

			if (bestNeighborID != InvalidPolygon)
			{
				PathPoint point;
				point.polygon = bestNeighborID;
				point.previous = currentPoint.polygon;
				point.pos = rayIntersection.point;
				CalculatePointData(point, currentPoint.g, currentPoint.pos, start);

				AddToOpenListIfBetter(openList, openMap, point);
			}
*/
			for (const NavMesh::InnerLinks::LinkData& link : navMesh.links.links[currentPoint.polygon])
			{
				// we already added bestNeighborID node to the open list
				// also skip the previous point where we came from
				if (/*link.neighbor == bestNeighborID ||*/ link.neighbor == currentPoint.previous)
				{
					continue;
				}

				Vector2D borderPointA = navMesh.geometry.vertices[link.border.first];
				Vector2D borderPointB = navMesh.geometry.vertices[link.border.second];
				Vector2D borderMiddlePoint = borderPointA + (borderPointB - borderPointA) * 0.5f;

				PathPoint point;
				point.polygon = link.neighbor;
				point.previous = currentPoint.polygon;
				point.pos = borderMiddlePoint;
				CalculatePointData(point, currentPoint.g, currentPoint.pos, start);

				AddToOpenListIfBetter(openList, openMap, point);
			}
		}

		outPath.push_back(start);
		size_t nextPolygon = currentPoint.polygon;
		while (nextPolygon != finishPolygon)
		{
			PathPoint point = closedList[nextPolygon];
			outPath.push_back(point.pos);
			nextPolygon = point.previous;
		}
		outPath.push_back(finish);

		// optimize the path
	}
} // namespace PathFinding
