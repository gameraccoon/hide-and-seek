#pragma once

#include "ECS/Entity.h"

#include "GameData/Spatial/CellPos.h"

struct SpatialEntityID
{
	Entity entity;
	CellPos cell;

	SpatialEntityID(Entity entity, CellPos cellPos);
};
