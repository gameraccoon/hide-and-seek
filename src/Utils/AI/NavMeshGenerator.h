#pragma once

#include <memory>
#include "GameData/AI/NavMesh.h"

#include <vector>

class CollisionComponent;
class TransformComponent;

class NavMeshGenerator
{
public:
	NavMeshGenerator();
	~NavMeshGenerator();

	void generateNavMesh(NavMesh& outNavMesh, const std::vector<std::tuple<CollisionComponent*, TransformComponent*>>& collidableComponents);

private:
	// caches that can be reused
	struct Caches;

private:
	std::unique_ptr<Caches> mCaches;
};
