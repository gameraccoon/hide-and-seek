#include "Utils/Geometry/NavMeshGenerator.h"

#include <polypartition.h>

#include <Components/CollisionComponent.generated.h>
#include <Components/TransformComponent.generated.h>


struct NavMeshGenerator::Caches
{
	std::vector<TPPLPoly> polygons;
	std::vector<TPPLPoly> resultPolygons;
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
	Vector2D size(10000, 10000);

	TPPLPartition pp;

	mCaches->polygons.clear();
	mCaches->resultPolygons.clear();

	mCaches->borderPolygon.Init(4);
	mCaches->borderPolygon[0].x = -size.x*0.5f;
	mCaches->borderPolygon[0].y = -size.y*0.5f;
	mCaches->borderPolygon[1].x = size.x*0.5f;
	mCaches->borderPolygon[1].y = -size.y*0.5f;
	mCaches->borderPolygon[2].x = size.x*0.5f;
	mCaches->borderPolygon[2].y = size.y*0.5f;
	mCaches->borderPolygon[3].x = -size.x*0.5f;
	mCaches->borderPolygon[3].y = size.y*0.5f;
	mCaches->polygons.push_back(mCaches->borderPolygon);

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
				const Vector2D& point = geometry.points[size - i];
				polygon[i].x = point.x + location.x;
				polygon[i].y = point.y + location.y;
			}
			polygon.Invert();
			polygon.SetHole(true);
			mCaches->polygons.push_back(polygon);
		}
	}

	mCaches->resultPolygons.clear();
	mCaches->resultPolygons.reserve(mCaches->polygons.size() * 5);
	pp.Triangulate_MONO(&mCaches->polygons, &mCaches->resultPolygons);

	outNavMesh.mTriangles.resize(mCaches->resultPolygons.size());

	for (size_t i = 0; i < mCaches->resultPolygons.size(); ++i)
	{
		const TPPLPoly& poly = mCaches->resultPolygons[i];
		std::array<Vector2D, 3>& resultPoly = outNavMesh.mTriangles[i];
		resultPoly[0].x = poly[0].x;
		resultPoly[0].y = poly[0].y;
		resultPoly[1].x = poly[1].x;
		resultPoly[1].x = poly[1].x;
		resultPoly[2].x = poly[2].x;
		resultPoly[2].x = poly[2].x;
	};
}
