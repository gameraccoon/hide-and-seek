#pragma once

#include "GameData/Core/Vector2D.h"
#include "GameData/Spatial/CellPos.h"
#include "GameData/Time/GameplayTimestamp.h"

struct DebugDrawPrimitive
{
	GameplayTimestamp lifeDeadline;

	DebugDrawPrimitive(GameplayTimestamp lifeDeadline)
		: lifeDeadline(lifeDeadline)
	{
	}

	bool isLifeTimeExceeded(GameplayTimestamp now) const
	{
		return now > lifeDeadline;
	}
};

struct DebugDrawWorldPoint : DebugDrawPrimitive
{
	CellPos cellPos;
	Vector2D pos;
	std::string name;

	DebugDrawWorldPoint(const CellPos& cellPos, const Vector2D& pos, GameplayTimestamp lifeDeadline)
		: DebugDrawPrimitive(lifeDeadline)
		, cellPos(cellPos)
		, pos(pos)
	{
	}

	DebugDrawWorldPoint(const CellPos& cellPos, const Vector2D& pos, const std::string& name, GameplayTimestamp lifeDeadline)
		: DebugDrawPrimitive(lifeDeadline)
		, cellPos(cellPos)
		, pos(pos)
		, name(name)
	{
	}
};

struct DebugDrawScreenPoint : DebugDrawPrimitive
{
	Vector2D screenPos;
	std::string name;
	GameplayTimestamp lifeDeadline;

	DebugDrawScreenPoint(const Vector2D& screenPos, GameplayTimestamp lifeDeadline)
		: DebugDrawPrimitive(lifeDeadline)
		, screenPos(screenPos)
	{
	}

	DebugDrawScreenPoint(const Vector2D& screenPos, const std::string& name, GameplayTimestamp lifeDeadline)
		: DebugDrawPrimitive(lifeDeadline)
		, screenPos(screenPos)
		, name(name)
	{
	}
};

struct DebugDrawWorldLineSegment : DebugDrawPrimitive
{
	Vector2D startPos;
	Vector2D endPos;
	GameplayTimestamp lifeDeadline;

	DebugDrawWorldLineSegment(const Vector2D& startPos, const Vector2D& endPos, GameplayTimestamp lifeDeadline)
		: DebugDrawPrimitive(lifeDeadline)
		, startPos(startPos)
		, endPos(endPos)
	{
	}
};
