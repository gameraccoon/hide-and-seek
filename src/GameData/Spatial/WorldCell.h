#pragma once

#include "ECS/EntityManager.h"
#include "ECS/ComponentSetHolder.h"

#include "GameData/Core/Vector2D.h"
#include "GameData/Spatial/CellPos.h"

struct ComponentSerializersHolder;

class WorldCell
{
public:
	explicit WorldCell(const CellPos& pos);

	EntityManager& getEntityManager() { return mEntityManager; }
	const EntityManager& getEntityManager() const { return mEntityManager; }
	ComponentSetHolder& getCellComponents() { return mCellComponents; }

	CellPos getPos() const { return mPos; }

	nlohmann::json toJson(const ComponentSerializersHolder& componentSerializers) const;
	void fromJson(const nlohmann::json& json, const ComponentSerializersHolder& componentSerializers);

private:
	EntityManager mEntityManager;
	ComponentSetHolder mCellComponents;
	CellPos mPos;
};
