#include "Utils/Geometry/NavMeshGenerator.h"

#include <unordered_map>

#include <polypartition.h>

#include <DetourNavMesh.h>
#include <DetourNavMeshBuilder.h>

#include <Debug/Log.h>

#include <Components/CollisionComponent.generated.h>
#include <Components/TransformComponent.generated.h>

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

void NavMeshGenerator::generateNavMesh(NavMesh& outNavMesh, const std::vector<std::tuple<CollisionComponent*, TransformComponent*>>& collidableComponents)
{
	using DtCoordType = unsigned short;
	using DtIndexType = unsigned short;
	using DtFlagsType = unsigned short;
	using DtAreaType = unsigned char;

	constexpr int polygonMaxVertsCount = 3;

	Vector2D size(10000, 10000);

	TPPLPartition pp;

	std::vector<TPPLPoly> polygons;
	std::vector<TPPLPoly> resultPolygons;

	mCaches->borderPolygon.Init(4);
	mCaches->borderPolygon[0].x = -size.x*0.5f;
	mCaches->borderPolygon[0].y = -size.y*0.5f;
	mCaches->borderPolygon[1].x = size.x*0.5f;
	mCaches->borderPolygon[1].y = -size.y*0.5f;
	mCaches->borderPolygon[2].x = size.x*0.5f;
	mCaches->borderPolygon[2].y = size.y*0.5f;
	mCaches->borderPolygon[3].x = -size.x*0.5f;
	mCaches->borderPolygon[3].y = size.y*0.5f;
	polygons.push_back(mCaches->borderPolygon);

	for (auto [collision, transform] : collidableComponents)
	{
		Vector2D location = transform->getLocation();
		const Hull& geometry = collision->getGeometry();
		if (geometry.type == Hull::Type::Angular)
		{
			TPPLPoly polygon;
			polygon.Init(geometry.points.size());

			size_t size = geometry.points.size();
			for (size_t i = 0; i < size; ++i)
			{
				const Vector2D& point = geometry.points[size - 1 - i];
				polygon[i].x = point.x + location.x;
				polygon[i].y = point.y + location.y;
			}
			polygon.SetHole(true);
			polygons.push_back(polygon);
		}
	}

	pp.Triangulate_MONO(&polygons, &resultPolygons);

	std::unordered_map<Vector2D, DtIndexType> verticesMap;
	for (const TPPLPoly& polygon : resultPolygons)
	{
		verticesMap.emplace(Vector2D(polygon[0].x, polygon[0].y), 0);
		verticesMap.emplace(Vector2D(polygon[1].x, polygon[1].y), 0);
		verticesMap.emplace(Vector2D(polygon[2].x, polygon[2].y), 0);
	}

	std::vector<DtCoordType> verts;
	verts.resize(verticesMap.size() * polygonMaxVertsCount);
	DtIndexType idx = 0;
	for (auto& it : verticesMap)
	{
		verts[idx * 3] = static_cast<DtCoordType>(it.first.x + 5000.0f);
		verts[idx * 3 + 1] = 0;
		verts[idx * 3 + 2] = static_cast<DtCoordType>(it.first.y + 5000.0f);
		it.second = idx++;
	}

	std::vector<DtIndexType> triangles;
	triangles.reserve(resultPolygons.size() * 2 * polygonMaxVertsCount);
	for (const TPPLPoly& polygon : resultPolygons)
	{
		triangles.push_back(verticesMap.find(Vector2D(polygon[0].x, polygon[0].y))->second);
		triangles.push_back(verticesMap.find(Vector2D(polygon[1].x, polygon[1].y))->second);
		triangles.push_back(verticesMap.find(Vector2D(polygon[2].x, polygon[2].y))->second);

		// borders-portals information
		triangles.push_back(0);
		triangles.push_back(0);
		triangles.push_back(0);
	}
	std::vector<DtFlagsType> flags(resultPolygons.size(), 0);
	std::vector<DtAreaType> areas(resultPolygons.size(), 0);

	outNavMesh.setMesh(new dtNavMesh());
	dtNavMeshCreateParams params;
	params.bmax[0] = size.x*0.5f;
	params.bmax[1] = 0.0f;
	params.bmax[2] = size.y*0.5f;
	params.bmin[0] = -size.x*0.5f;
	params.bmin[1] = 0.0f;
	params.bmin[2] = -size.y*0.5f;
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
	params.walkableRadius = 1.0f;

	unsigned char* navData = nullptr;
	int navDataSize = 0;
	bool isSuccessful = dtCreateNavMeshData(&params, &navData, &navDataSize);
	if (!isSuccessful)
	{
		LogInfo("Unsuccessfull detour navmesh generation");
		return;
	}

	dtNavMeshParams navmeshParams;
	navmeshParams.maxPolys = static_cast<int>(resultPolygons.size());
	navmeshParams.maxTiles = 1;
	navmeshParams.orig[0] = -5000.0f;
	navmeshParams.orig[1] = 0.0f;
	navmeshParams.orig[2] = -5000.0f;
	navmeshParams.tileHeight = 10000;
	navmeshParams.tileWidth = 10000;
	outNavMesh.getMesh()->init(&navmeshParams);
	outNavMesh.getMesh()->addTile(navData, navDataSize, 0, 0, nullptr);
}
