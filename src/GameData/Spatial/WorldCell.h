#pragma once

#include "ECS/EntityManager.h"
#include "ECS/ComponentSetHolder.h"

class WorldCell
{
public:
	using UniquePtr = std::unique_ptr<WorldCell>;
	static const unsigned int CellSize = 1000.0f;

public:
	EntityManager& getEntityManager() { return mEntityManager; }
	ComponentSetHolder& getCellComponents() { return mCellComponents; }

	nlohmann::json toJson(const ComponentFactory& componentFactory) const;
	void fromJson(const nlohmann::json& json, const ComponentFactory& componentFactory);

private:
	EntityManager mEntityManager;
	ComponentSetHolder mCellComponents;
};
