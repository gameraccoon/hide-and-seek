#pragma once

#include <vector>

#include "GameData/Core/Vector2D.h"
#include "GameData/Core/IntVector2D.h"

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
		size_t vertsPerPoly = 3;
		size_t polygonsCount = 0;
		Vector2D navMeshStart{ZERO_VECTOR};
		Vector2D navMeshSize{ZERO_VECTOR};
		bool isCalculated = false;
	};

	struct InnerLinks
	{
		struct LinkData
		{
			LinkData() = default;
			LinkData(size_t neighbor, const std::pair<size_t, size_t>& border)
				: neighbor(neighbor)
				, border(border)
			{}

			size_t neighbor;
			std::pair<size_t, size_t> border;
		};

		std::vector<std::vector<LinkData>> links;
		bool isCalculated = false;
	};

	struct SpatialHash
	{
		float cellSize = 100.0f;
		IntVector2D hashSize;
		std::vector<std::vector<size_t>> polygonsHash;
		bool isCalculated = false;
	};

	Geometry geometry;
	InnerLinks links;
	SpatialHash spatialHash;
};
