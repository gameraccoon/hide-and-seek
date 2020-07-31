#pragma once

#include <vector>

#include "GameData/Core/Vector2D.h"

struct NavMesh
{
	NavMesh() = default;
	// NavMesh is a very heavy class, copying it does not have much sense
	NavMesh(const NavMesh&) = delete;
	NavMesh& operator=(const NavMesh&) = delete;

	struct Geometry
	{
		std::vector<Vector2D> vertices;
		std::vector<size_t> indexes;
		size_t polygonMaxVerticesCount = 3;
		size_t polygonsCount = 0;
		Vector2D navMeshStart{ZERO_VECTOR};
		Vector2D navMeshSize{ZERO_VECTOR};
		bool isCalculated = false;
	};

	struct InnerLinks
	{
		std::vector<std::vector<size_t>> links;
		bool isCalculated = false;
	};

	struct SpatialHash
	{
		float spatialHashCellSize = 10.0f;
		std::vector<std::vector<size_t>> trianlesHash;
		bool isCalculated = false;
	};

	Geometry geometry;
	InnerLinks links;
	SpatialHash spatialHash;
};
