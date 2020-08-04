#include "Base/precomp.h"

#include "Utils/AI/PathFinding.h"

#include "GameData/AI/NavMesh.h"
#include "GameData/Core/BoundingBox.h"

#include "Utils/Geometry/Collide.h"

namespace PathFinding
{
	static constexpr size_t InvalidPolygon = std::numeric_limits<size_t>::max();

	bool IsPointInsideConvexHull(Vector2D point, const std::vector<Vector2D>& hull)
	{
		if (Collide::SignedArea(point, hull[hull.size() - 1], hull[0]) < 0.0f)
		{
			return false;
		}

		for (size_t i = 0; i < hull.size() - 1; ++i)
		{
			if (Collide::SignedArea(point, hull[i], hull[i+1]) < 0.0f)
			{
				return false;
			}
		}
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

	void FindPath(std::vector<Vector2D>& outPath, const NavMesh& navMesh, Vector2D start, Vector2D finish)
	{
		outPath.clear();

		size_t startPolygon = FindPolygonForPoint(start, navMesh);
		size_t finishPolygon = FindPolygonForPoint(finish, navMesh);

		if (startPolygon == InvalidPolygon || finishPolygon == InvalidPolygon)
		{
			return;
		}

		outPath.push_back(start);
		outPath.push_back(finish);

		// while steps size less than the limit
		{
			// if this the last polygon exit the loop
			// add all the neighbours of the polygon to the open list
			// add the polygon to the closed list
			// raycast to the target
			// find the border that intersects with the ray
			// is there a neighbor on the other side of the border?
			// yes? add the point to the final list, restart the cycle with the neighbor
			// no?
			// get a polygon from the open list
		}

		// optimize the path
	}
} // namespace PathFinding
