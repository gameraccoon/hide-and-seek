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

void SpatialEntityManager::getSpatialEntitiesHavingComponents(const std::vector<std::type_index>& componentIndexes, TupleVector<WorldCell*, Entity>& inOutEntities) const
{
	for (WorldCell* cell : mCells)
	{
		std::vector<Entity> entities;
		cell->getEntityManager().getEntitiesHavingComponents(componentIndexes, entities);
		std::transform(
			entities.begin(),
			entities.end(),
			std::back_inserter(inOutEntities),
			[cell](Entity entity)
			{
				return std::make_tuple(cell, entity);
			}
		);
	}
}

WorldCell* SpatialEntityManager::findEntityCell(Entity entity)
{
	for (WorldCell* cell : mCells)
	{
		if (cell->getEntityManager().hasEntity(entity))
		{
			return cell;
		}
	}
	return nullptr;
}
