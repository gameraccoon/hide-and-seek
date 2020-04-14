#pragma once

#include "GameData/Core/Vector2D.h"
#include "GameData/Spatial/CellPos.h"

struct SpatialPoint
{
	SpatialPoint() = default;
	SpatialPoint(Vector2D pos, CellPos cellPos)
		: pos(pos)
		, cellPos(cellPos)
	{}

	Vector2D pos;
	CellPos cellPos;
};
