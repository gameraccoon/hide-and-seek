#pragma once

#include <vector>

#include "ECS/EntityManager.h"
#include "ECS/EntityView.h"

#include "GameData/Spatial/WorldCell.h"

class SpatialEntityManager
{
public:
	explicit SpatialEntityManager(const std::vector<WorldCell*>& cells);

	template<typename FirstComponent, typename... Components>
	void getComponents(TupleVector<FirstComponent*, Components*...>& inOutComponents)
	{
		for (WorldCell* cell : mCells)
		{
			cell->getEntityManager().getComponents<FirstComponent, Components...>(inOutComponents);
		}
	}

	template<typename FirstComponent, typename... Components>
	void getSpatialComponents(TupleVector<WorldCell*, FirstComponent*, Components*...>& inOutComponents)
	{
		for (WorldCell* cell : mCells)
		{
			cell->getEntityManager().getComponents<FirstComponent, Components...>(inOutComponents, cell);
		}
	}

	template<typename FirstComponent, typename... Components>
	void getSpatialComponentsWithEntities(TupleVector<Entity, WorldCell*, FirstComponent*, Components*...>& inOutComponents)
	{
		for (WorldCell* cell : mCells)
		{
			cell->getEntityManager().getComponentsWithEntities<FirstComponent, Components...>(inOutComponents, cell);
		}
	}

	template<typename FirstComponent, typename... Components, typename FunctionType>
	void forEachComponentSet(FunctionType processor)
	{
		for (WorldCell* cell : mCells)
		{
			cell->getEntityManager().forEachComponentSet<FirstComponent, Components...>(processor);
		}
	}

	template<typename FirstComponent, typename... Components, typename FunctionType>
	void forEachSpatialComponentSet(FunctionType processor)
	{
		for (WorldCell* cell : mCells)
		{
			cell->getEntityManager().forEachComponentSet<FirstComponent, Components...>(processor, cell);
		}
	}

	template<typename FirstComponent, typename... Components, typename FunctionType>
	void forEachSpatialComponentSetWithEntity(FunctionType processor)
	{
		for (WorldCell* cell : mCells)
		{
			cell->getEntityManager().forEachComponentSetWithEntity<FirstComponent, Components...>(processor, cell);
		}
	}

	void executeScheduledActions();

	void getAllEntityComponents(Entity entity, std::vector<BaseComponent*>& outComponents);

	void getEntitiesHavingComponents(const std::vector<std::type_index>& componentIndexes, std::vector<Entity>& inOutEntities) const;

private:
	std::vector<WorldCell*> mCells;
};
