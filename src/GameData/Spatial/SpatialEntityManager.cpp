#include "Base/precomp.h"

#include "GameData/Spatial/SpatialEntityManager.h"

SpatialEntityManager::SpatialEntityManager(const std::vector<WorldCell*>& cells)
	: mCells(cells)
{
}

void SpatialEntityManager::executeScheduledActions()
{
	for (WorldCell* cell : mCells)
	{
		cell->getEntityManager().executeScheduledActions();
	}
}

void SpatialEntityManager::getAllEntityComponents(Entity entity, std::vector<BaseComponent*>& outComponents)
{
	for (WorldCell* cell : mCells)
	{
		cell->getEntityManager().getAllEntityComponents(entity, outComponents);

		if (!outComponents.empty())
		{
			break;
		}
	}
}

void SpatialEntityManager::getEntitiesHavingComponents(const std::vector<std::type_index>& componentIndexes, std::vector<Entity>& inOutEntities) const
{
	for (WorldCell* cell : mCells)
	{
		cell->getEntityManager().getEntitiesHavingComponents(componentIndexes, inOutEntities);
	}
}
