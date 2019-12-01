#pragma once

#include "ECS/EntityManager.h"
#include "ECS/ComponentSetHolder.h"

#include "GameData/Core/Vector2D.h"

class WorldCell
{
public:
	EntityManager& getEntityManager() { return mEntityManager; }
	ComponentSetHolder& getCellComponents() { return mCellComponents; }

	nlohmann::json toJson(const ComponentFactory& componentFactory) const;
	void fromJson(const nlohmann::json& json, const ComponentFactory& componentFactory);

private:
	EntityManager mEntityManager;
	ComponentSetHolder mCellComponents;
};
