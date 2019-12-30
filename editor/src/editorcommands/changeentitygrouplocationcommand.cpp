#include "changeentitygrouplocationcommand.h"

#include <GameData/World.h>
#include <GameData/Components/TransformComponent.generated.h>

ChangeEntityGroupLocationCommand::ChangeEntityGroupLocationCommand(const std::vector<SpatialEntity>& entities, const Vector2D& shift)
	: mEntities(entities)
	, mShift(shift)
{
}

bool ChangeEntityGroupLocationCommand::doCommand(World* world)
{
	mEntitiesCellPos.resize(mEntities.size());
	for (size_t i = 0; i < mEntities.size(); ++i)
	{
		const SpatialEntity& spatialEntity = mEntities[i];
		mEntitiesCellPos[i] = spatialEntity.cell;
		if (WorldCell* cell = world->getSpatialData().getCell(spatialEntity.cell))
		{
			auto [component] = cell->getEntityManager().getEntityComponents<TransformComponent>(spatialEntity.entity.getEntity());
			if (component)
			{
				auto [newCellPos, newLocation] = SpatialWorldData::GetTransformedCellPos(spatialEntity.cell, component->getLocation() + mShift);
				component->setLocation(newLocation);

				if (newCellPos != spatialEntity.cell)
				{
					mEntitiesCellPos[i] = newCellPos;
					WorldCell& newCell = world->getSpatialData().getOrCreateCell(newCellPos);
					cell->getEntityManager().transferEntityTo(newCell.getEntityManager(), spatialEntity.entity.getEntity());
				}
			}
		}
	}
	return true;
}

bool ChangeEntityGroupLocationCommand::undoCommand(World* world)
{
	for (size_t i = 0; i < mEntities.size(); ++i)
	{
		const SpatialEntity& spatialEntity = mEntities[i];
		CellPos oldCellPos = mEntitiesCellPos[i];
		if (WorldCell* cell = world->getSpatialData().getCell(oldCellPos))
		{
			auto [component] = cell->getEntityManager().getEntityComponents<TransformComponent>(spatialEntity.entity.getEntity());
			if (component)
			{
				auto [newCellPos, newLocation] = SpatialWorldData::GetTransformedCellPos(spatialEntity.cell, component->getLocation() - mShift);
				component->setLocation(newLocation);

				if (oldCellPos != spatialEntity.cell)
				{
					Assert(spatialEntity.cell == newCellPos, "Inconsistent undo/redo transformations in ChangeEntityGroupLocationCommand");
					WorldCell& newCell = world->getSpatialData().getOrCreateCell(spatialEntity.cell);
					cell->getEntityManager().transferEntityTo(newCell.getEntityManager(), spatialEntity.entity.getEntity());
				}
			}
		}
	}
	return true;
}

EditorCommand::EffectType ChangeEntityGroupLocationCommand::getEffectType()
{
	return EffectType::ComponentAttributes;
}
