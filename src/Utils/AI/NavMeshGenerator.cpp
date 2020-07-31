#include "Base/precomp.h"

#include "Utils/AI/NavMeshGenerator.h"

#include <cmath>
#include <unordered_map>

#include <polypartition.h>

#include "Base/Types/TemplateAliases.h"

#include "GameData/Components/CollisionComponent.generated.h"
#include "GameData/Components/TransformComponent.generated.h"
#include "GameData/Core/IntVector2D.h"
#include "GameData/AI/NavMesh.h"

namespace NavMeshGenerator
{
	static IntVector2D IntVecFromTPPLPoint(TPPLPoint point)
	{
		return IntVector2D(static_cast<int>(std::round(point.x)), static_cast<int>(std::round(point.y)));
	}

	void GenerateNavMeshGeometry(NavMesh::Geometry& outGeometry, const TupleVector<CollisionComponent*, TransformComponent*>& collidableComponents, Vector2D start, Vector2D size)
	{
		outGeometry.vertices.clear();
		outGeometry.indexes.clear();
		// only triangles are supported now
		outGeometry.polygonMaxVerticesCount = 3;
		outGeometry.isCalculated = false;

		IntVector2D halfSize(static_cast<int>(size.x * 0.5f), static_cast<int>(size.y * 0.5f));
		IntVector2D centerPos(static_cast<int>(start.x) + halfSize.x, static_cast<int>(start.y) + halfSize.y);

		std::vector<TPPLPoly> polygons;
		std::vector<TPPLPoly> resultPolygons;

		TPPLPoly borderPolygon;
		borderPolygon.Init(4);
		borderPolygon[0].x = centerPos.x - halfSize.x;
		borderPolygon[0].y = centerPos.y - halfSize.y;
		borderPolygon[1].x = centerPos.x + halfSize.x;
		borderPolygon[1].y = centerPos.y - halfSize.y;
		borderPolygon[2].x = centerPos.x + halfSize.x;
		borderPolygon[2].y = centerPos.y + halfSize.y;
		borderPolygon[3].x = centerPos.x - halfSize.x;
		borderPolygon[3].y = centerPos.y + halfSize.y;
		polygons.push_back(borderPolygon);

		for (auto [collision, transform] : collidableComponents)
		{
			Vector2D location = transform->getLocation();
			const Hull& geometry = collision->getGeometry();
			if (geometry.type == HullType::Angular)
			{
				TPPLPoly polygon;
				polygon.Init(geometry.points.size());

				size_t pointsSize = geometry.points.size();
				for (size_t i = 0; i < pointsSize; ++i)
				{
					const Vector2D& point = geometry.points[pointsSize - 1 - i];
					polygon[i].x = point.x + location.x;
					polygon[i].y = point.y + location.y;
				}
				polygon.SetHole(true);
				polygons.push_back(polygon);
			}
		}

		TPPLPartition pp;
		pp.Triangulate_MONO(&polygons, &resultPolygons);

		std::unordered_map<IntVector2D, size_t> verticesMap;
		for (const TPPLPoly& polygon : resultPolygons)
		{
			verticesMap.emplace(IntVecFromTPPLPoint(polygon[0]), static_cast<size_t>(0u));
			verticesMap.emplace(IntVecFromTPPLPoint(polygon[1]), static_cast<size_t>(0u));
			verticesMap.emplace(IntVecFromTPPLPoint(polygon[2]), static_cast<size_t>(0u));
		}

		outGeometry.vertices.resize(verticesMap.size());
		size_t idx = 0;
		for (auto& it : verticesMap)
		{
			outGeometry.vertices[idx].x = static_cast<float>(it.first.x);
			outGeometry.vertices[idx].y = static_cast<float>(it.first.y);
			it.second = idx++;
		}

		outGeometry.indexes.reserve(resultPolygons.size() * outGeometry.polygonMaxVerticesCount);
		for (const TPPLPoly& polygon : resultPolygons)
		{
			outGeometry.indexes.push_back(verticesMap.find(IntVecFromTPPLPoint(polygon[0]))->second);
			outGeometry.indexes.push_back(verticesMap.find(IntVecFromTPPLPoint(polygon[1]))->second);
			outGeometry.indexes.push_back(verticesMap.find(IntVecFromTPPLPoint(polygon[2]))->second);
		}

		outGeometry.polygonsCount = resultPolygons.size();
		outGeometry.navMeshStart = start;
		outGeometry.navMeshSize = size;
		outGeometry.isCalculated = true;
	}

	struct size_t_pair_hash
	{
		std::size_t operator() (const std::pair<size_t, size_t> &p) const
		{
			return std::hash<size_t>()(p.first) ^ (std::hash<size_t>()(p.second) << 1);
		}
	};

	static std::pair<size_t, size_t> makeSortedPair(size_t first, size_t second)
	{
		return (first < second) ? std::make_pair(first, second) : std::make_pair(second, first);
	}

	void LinkNavMesh(NavMesh::InnerLinks& outLinks, const NavMesh::Geometry& geometry)
	{
		Assert(geometry.isCalculated, "Geometry should be calculated before calculating links");

		// form a dictionary with borders as keys and polygon indexes as values
		std::unordered_map<std::pair<size_t, size_t>, std::vector<size_t>, size_t_pair_hash> trianglesByBorders;
		for (size_t i = 0, iSize = geometry.polygonsCount; i < iSize; ++i)
		{
			for (size_t j = 0, jSize = geometry.polygonMaxVerticesCount - 1; j < jSize; ++j)
			{
				size_t indexA = geometry.indexes[i * geometry.polygonMaxVerticesCount + j];
				size_t indexB = geometry.indexes[i * geometry.polygonMaxVerticesCount + j + 1];
				trianglesByBorders[makeSortedPair(indexA, indexB)].push_back(i);
			}

			{
				// last border
				size_t indexA = geometry.indexes[i * geometry.polygonMaxVerticesCount + 0];
				size_t indexB = geometry.indexes[i * geometry.polygonMaxVerticesCount + geometry.polygonMaxVerticesCount - 1];
				trianglesByBorders[makeSortedPair(indexA, indexB)].push_back(i);
			}
		}

		// connect polygons that have a shared border
		outLinks.links.clear();
		outLinks.links.resize(geometry.polygonsCount);
		for (auto& pair : trianglesByBorders)
		{
			Assert(pair.second.size() <= 2, "There are more than 2 triangles that share the same border. That should not happen.");
			if (pair.second.size() == 2)
			{
				outLinks.links[pair.second[0]].push_back(pair.second[1]);
				outLinks.links[pair.second[1]].push_back(pair.second[0]);
			}
		}

		outLinks.isCalculated = true;
	}

	void BuildSpatialHash(NavMesh::SpatialHash& outSpatialHash, const NavMesh::Geometry& geometry)
	{
		Assert(geometry.isCalculated, "Geometry should be calculated before calculating spatial hash");

		outSpatialHash.isCalculated = true;
	}

} // namespace NavMeshGenerator
