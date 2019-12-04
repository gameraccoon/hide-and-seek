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
	void getComponents(std::vector<std::tuple<FirstComponent*, Components*...>>& inOutComponents)
	{
		for (WorldCell* cell : mCells)
		{
			cell->getEntityManager().getComponents<FirstComponent, Components...>(inOutComponents);
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
			cell->getEntityManager().forEachComponentSet<FirstComponent, Components...>([&processor, cell](FirstComponent* first, Components*... components)
			{
				processor(cell, first, components...);
			});
		}
	}

	template<typename FirstComponent, typename... Components, typename FunctionType>
	void forEachComponentSetWithEntity(FunctionType processor)
	{
		for (WorldCell* cell : mCells)
		{
			cell->getEntityManager().forEachComponentSetWithEntity<FirstComponent, Components...>([cell, &processor](Entity entity, FirstComponent* first, Components*... components)
			{
				processor(EntityView(entity, cell->getEntityManager()), first, components...);
			});
		}
	}

	void executeScheduledActions();

private:
	std::vector<WorldCell*> mCells;
};
