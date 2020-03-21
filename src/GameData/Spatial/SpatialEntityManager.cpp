#include "Base/precomp.h"

#include "GameData/Spatial/SpatialEntityManager.h"

#include "Base/Debug/Assert.h"

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
