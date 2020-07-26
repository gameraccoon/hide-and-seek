#include "Base/precomp.h"

#include "Utils/AI/NavMeshGenerator.h"

#include <cmath>
#include <unordered_map>

#include <polypartition.h>

#include <DetourNavMesh.h>
#include <DetourNavMeshBuilder.h>

#include "Base/Types/TemplateAliases.h"
#include "Base/Types/ComplexTypes/VectorUtils.h"

#include "GameData/Components/CollisionComponent.generated.h"
#include "GameData/Components/TransformComponent.generated.h"
#include "GameData/Core/IntVector2D.h"

struct NavMeshGenerator::Caches
{
	TPPLPoly borderPolygon;
};

NavMeshGenerator::NavMeshGenerator()
{
	mCaches = std::make_unique<Caches>();
}

NavMeshGenerator::~NavMeshGenerator()
{
}

static IntVector2D IntVecFromTPPLPoint(TPPLPoint point)
{
	return IntVector2D(static_cast<int>(std::round(point.x)), static_cast<int>(std::round(point.y)));
}

void NavMeshGenerator::generateNavMesh(NavMesh& outNavMesh, const TupleVector<CollisionComponent*, TransformComponent*>& collidableComponents)
{
	using DtCoordType = unsigned short;
	using DtIndexType = unsigned short;
	using DtFlagsType = unsigned short;
	using DtAreaType = unsigned char;

	constexpr int polygonMaxVertsCount = 3;

	IntVector2D size(10000, 10000);
	IntVector2D halfSize(size / 2);

	TPPLPartition pp;

	std::vector<TPPLPoly> polygons;
	std::vector<TPPLPoly> resultPolygons;

	mCaches->borderPolygon.Init(4);
	mCaches->borderPolygon[0].x = -halfSize.x;
	mCaches->borderPolygon[0].y = -halfSize.y;
	mCaches->borderPolygon[1].x = halfSize.x;
	mCaches->borderPolygon[1].y = -halfSize.y;
	mCaches->borderPolygon[2].x = halfSize.x;
	mCaches->borderPolygon[2].y = halfSize.y;
	mCaches->borderPolygon[3].x = -halfSize.x;
	mCaches->borderPolygon[3].y = halfSize.y;
	polygons.push_back(mCaches->borderPolygon);

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

	pp.Triangulate_MONO(&polygons, &resultPolygons);

	std::unordered_map<IntVector2D, DtIndexType> verticesMap;
	for (const TPPLPoly& polygon : resultPolygons)
	{
		verticesMap.emplace(IntVecFromTPPLPoint(polygon[0]), static_cast<DtIndexType>(0u));
		verticesMap.emplace(IntVecFromTPPLPoint(polygon[1]), static_cast<DtIndexType>(0u));
		verticesMap.emplace(IntVecFromTPPLPoint(polygon[2]), static_cast<DtIndexType>(0u));
	}

	std::vector<DtCoordType> verts;
	verts.resize(verticesMap.size() * polygonMaxVertsCount);
	DtIndexType idx = 0;
	for (auto& it : verticesMap)
	{
		verts[static_cast<size_t>(idx) * 3] = static_cast<DtCoordType>(it.first.x + halfSize.x);
		verts[static_cast<size_t>(idx) * 3 + 1] = 0;
		verts[static_cast<size_t>(idx) * 3 + 2] = static_cast<DtCoordType>(it.first.y + halfSize.y);
		it.second = idx++;
	}

	std::vector<DtIndexType> triangles;
	triangles.reserve(resultPolygons.size() * 2 * polygonMaxVertsCount);
	for (const TPPLPoly& polygon : resultPolygons)
	{
		triangles.push_back(verticesMap.find(IntVecFromTPPLPoint(polygon[0]))->second);
		triangles.push_back(verticesMap.find(IntVecFromTPPLPoint(polygon[1]))->second);
		triangles.push_back(verticesMap.find(IntVecFromTPPLPoint(polygon[2]))->second);

		// borders-portals information
		triangles.push_back(0);
		triangles.push_back(0);
		triangles.push_back(0);
	}
	std::vector<DtFlagsType> flags(resultPolygons.size(), 1);
	std::vector<DtAreaType> areas(resultPolygons.size(), 0);

	outNavMesh.setMesh(std::make_unique<dtNavMesh>());
	dtNavMeshCreateParams params;
	params.bmax[0] = halfSize.x;
	params.bmax[1] = 0.0f;
	params.bmax[2] = halfSize.y;
	params.bmin[0] = -halfSize.x;
	params.bmin[1] = 0.0f;
	params.bmin[2] = -halfSize.y;
	params.buildBvTree = false;
	params.ch = 1.0f;
	params.cs = 1.0f;
	params.detailMeshes = nullptr;
	params.detailTriCount = 0;
	params.detailTris = nullptr;
	params.detailVerts = nullptr;
	params.detailVertsCount = 0;
	params.nvp = polygonMaxVertsCount;
	params.offMeshConAreas = nullptr;
	params.offMeshConCount = 0;
	params.offMeshConDir = nullptr;
	params.offMeshConFlags = nullptr;
	params.offMeshConRad = nullptr;
	params.offMeshConUserID = nullptr;
	params.offMeshConVerts = nullptr;
	params.polyAreas = areas.data();
	params.polyCount = static_cast<int>(resultPolygons.size());
	params.polyFlags = flags.data();
	params.polys = triangles.data();
	params.tileLayer = 0;
	params.tileX = 0;
	params.tileY = 0;
	params.userId = 0;
	params.verts = verts.data();
	params.vertCount = static_cast<int>(verticesMap.size());
	params.walkableClimb = 0.0f;
	params.walkableHeight = 10.0f;
	params.walkableRadius = 10.0f;

	unsigned char* navData = nullptr;
	int navDataSize = 0;
	bool isSuccessful = dtCreateNavMeshData(&params, &navData, &navDataSize);
	if (!isSuccessful)
	{
		LogInfo("Unsuccessfull detour navmesh generation");
		outNavMesh.setMesh(nullptr);
		return;
	}

	dtNavMeshParams navmeshParams;
	navmeshParams.maxPolys = static_cast<int>(resultPolygons.size());
	navmeshParams.maxTiles = 1;
	navmeshParams.orig[0] = -halfSize.x;
	navmeshParams.orig[1] = 0.0f;
	navmeshParams.orig[2] = -halfSize.y;
	navmeshParams.tileHeight = size.x;
	navmeshParams.tileWidth = size.y;
	outNavMesh.getMesh()->init(&navmeshParams);
	outNavMesh.getMesh()->addTile(navData, navDataSize, DT_TILE_FREE_DATA, 0, nullptr);
}
