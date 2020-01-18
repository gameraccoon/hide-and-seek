#pragma once

#include <memory>
#include <vector>

#include "Base/Types/TemplateAliases.h"

#include "GameData/AI/NavMesh.h"


class CollisionComponent;
class TransformComponent;

class NavMeshGenerator
{
public:
	NavMeshGenerator();
	~NavMeshGenerator();

	void generateNavMesh(NavMesh& outNavMesh, const TupleVector<CollisionComponent*, TransformComponent*>& collidableComponents);

private:
	// caches that can be reused
	struct Caches;

private:
	std::unique_ptr<Caches> mCaches;
};
