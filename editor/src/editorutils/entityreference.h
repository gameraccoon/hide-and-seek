#pragma once

#include <optional>

#include <ECS/Entity.h>

#include <GameData/Spatial/CellPos.h>

struct EntityReference
{
	Entity entity;
	/** nullopt indicates that the component is not bound to a cell */
	std::optional<CellPos> cellPos;

	explicit EntityReference(Entity entity): entity(entity) {}
};
