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

	struct LineSegmentToPolygonIntersection
	{
		size_t borderPointA;
		size_t borderPointB;
		Vector2D point;
	};

	static LineSegmentToPolygonIntersection FindLineSegmentToPolygonIntersection(const NavMesh::Geometry& geometry, Vector2D start, Vector2D finish, size_t polygon)
	{
		LineSegmentToPolygonIntersection result;

		size_t polygonShift = polygon * geometry.vertsPerPoly;
		FOR_EACH_BORDER(geometry.vertsPerPoly,
		{
			Vector2D vert1 = geometry.vertices[geometry.indexes[polygonShift + i]];
			Vector2D vert2 = geometry.vertices[geometry.indexes[polygonShift + j]];

			bool areIntersect = Collide::AreLinesIntersect(start, finish, vert1, vert2);

			if (areIntersect)
			{
				result.borderPointA = polygonShift + i;
				result.borderPointB = polygonShift + j;
				result.point = Collide::GetPointIntersect2Lines(start, finish, vert1, vert2);
				return result;
			}
		});

		return result;
	}

	size_t findNeighborWithBorder(const NavMesh::InnerLinks& links, size_t currentPolygon, size_t borderPointA, size_t borderPointB)
	{
		for (const NavMesh::InnerLinks::LinkData& link : links.links[currentPolygon])
		{
			if (link.border.first == borderPointA && link.border.second == borderPointB)
			{
				return link.neighbor;
			}
		}

		return InvalidPolygon;
	}

	struct PathPoint
	{
		size_t polygon;
		Vector2D pos;
		float g;
		float h;
		float f;
		size_t previous;
	};

	void FindPath(std::vector<Vector2D>& outPath, const NavMesh& navMesh, Vector2D start, Vector2D finish)
	{
		outPath.clear();

		size_t startPolygon = FindPolygonForPoint(start, navMesh);
		size_t finishPolygon = FindPolygonForPoint(finish, navMesh);

		if (startPolygon == InvalidPolygon || finishPolygon == InvalidPolygon)
		{
			return;
		}

		PathPoint currentPoint;
		currentPoint.polygon = startPolygon;
		currentPoint.previous = InvalidPolygon;
		currentPoint.g = 0.0f;
		currentPoint.h = (finish - start).size();
		currentPoint.f = currentPoint.g + currentPoint.h;
		currentPoint.pos = start;

		std::list<PathPoint> openList;
		std::vector<bool> openSet(navMesh.geometry.polygonsCount, false);
		std::unordered_map<size_t, PathPoint> closedList;

		unsigned int stepLimit = 100u;
		unsigned int step = 0u;
		while (step < stepLimit)
		{
			if (currentPoint.polygon == finishPolygon)
			{
				break;
			}

			for (const NavMesh::InnerLinks::LinkData& link : navMesh.links.links[currentPoint.polygon])
			{
				if (openSet[link.neighbor] == false)
				{
					Vector2D borderPointA = navMesh.geometry.vertices[link.border.first];
					Vector2D borderPointB = navMesh.geometry.vertices[link.border.second];
					Vector2D borderMiddlePoint = borderPointA + (borderPointB - borderPointA) * 0.5f;

					PathPoint point;
					point.polygon = link.neighbor;
					point.previous = currentPoint.polygon;
					point.g = currentPoint.g + (borderMiddlePoint - currentPoint.pos).size();
					openList.push_back(std::move(point));
					openSet[link.neighbor] = true;
				}
			}

			LineSegmentToPolygonIntersection intersection = FindLineSegmentToPolygonIntersection(navMesh.geometry, currentPoint.pos, finish, currentPoint.polygon);

			{
				float scoreG = currentPoint.g + (intersection.point - currentPoint.pos).size();
				auto [iterator, isEnserted] = closedList.emplace(currentPoint.polygon, PathPoint{});
				PathPoint& closedPoint = iterator->second;
				if (isEnserted || scoreG < closedPoint.g)
				{
					closedPoint.pos = intersection.point;
					closedPoint.h = (intersection.point - start).size();
					closedPoint.g = scoreG;
					closedPoint.f = closedPoint.h + closedPoint.g;
					closedPoint.previous = currentPoint.previous;
				}
			}

			size_t neighborID = findNeighborWithBorder(navMesh.links, currentPoint.polygon, intersection.borderPointA, intersection.borderPointB);

			if (neighborID != InvalidPolygon)
			{
				outPath.push_back(intersection.point);
//				currentPolygon = neighborID;
			}
			else
			{
//				currentPolygon = openList.pop_front();
			}

			++step;
		}

		outPath.push_back(start);
		// ToDo: rewind path from closed list
		outPath.push_back(finish);

		// optimize the path
	}
} // namespace PathFinding
