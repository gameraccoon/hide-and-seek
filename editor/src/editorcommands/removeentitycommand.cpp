#include "removeentitycommand.h"

#include <QtWidgets/qcombobox.h>

#include "ECS/ComponentFactory.h"
#include "GameData/World.h"

RemoveEntityCommand::RemoveEntityCommand(Entity entity, ComponentFactory* factory)
	: EditorCommand(EffectType::Entities)
	, mEntity(entity)
	, mComponentFactory(factory)
{
}

void RemoveEntityCommand::doCommand(World* world)
{
	if (mSerializedComponents.empty())
	{
		world->getEntityManager().getPrefabFromEntity(mSerializedComponents, mEntity);
	}

	world->getEntityManager().removeEntity(mEntity);
}

void RemoveEntityCommand::undoCommand(World* world)
{
	world->getEntityManager().insertEntityUnsafe(mEntity);
	world->getEntityManager().applyPrefabToExistentEntity(mSerializedComponents, mEntity, *mComponentFactory);
}
