#pragma once

#include "GameData/Core/Vector2D.h"
#include "GameData/Spatial/CellPos.h"

struct DebugDrawWorldPoint
{
	CellPos cellPos;
	Vector2D pos;
	std::string name;

	DebugDrawWorldPoint(const CellPos& cellPos, const Vector2D& pos)
		: cellPos(cellPos)
		, pos(pos)
	{
	}

	DebugDrawWorldPoint(const CellPos& cellPos, const Vector2D& pos, const std::string& name)
		: cellPos(cellPos)
		, pos(pos)
		, name(name)
	{
	}
};

struct DebugDrawScreenPoint
{
	Vector2D screenPos;
	std::string name;

	DebugDrawScreenPoint(const Vector2D& screenPos)
		: screenPos(screenPos)
	{
	}

	DebugDrawScreenPoint(const Vector2D& screenPos, const std::string& name)
		: screenPos(screenPos)
		, name(name)
	{
	}
};
