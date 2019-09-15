#include "removeentitycommand.h"

#include <QtWidgets/qcombobox.h>

#include "Debug/Assert.h"
#include "ECS/ComponentFactory.h"
#include "GameData/World.h"

RemoveEntityCommand::RemoveEntityCommand(Entity entity, ComponentFactory* factory)
	: mEntity(entity)
	, mComponentFactory(factory)
{
}

bool RemoveEntityCommand::doCommand(World* world)
{
	if (mSerializedComponents.empty())
	{
		world->getEntityManager().getPrefabFromEntity(mSerializedComponents, mEntity);
	}

	world->getEntityManager().removeEntity(mEntity);
	return false;
}

bool RemoveEntityCommand::undoCommand(World* world)
{
	world->getEntityManager().insertEntityUnsafe(mEntity);
	world->getEntityManager().applyPrefabToExistentEntity(mSerializedComponents, mEntity, *mComponentFactory);
	return false;
}

EditorCommand::EffectType RemoveEntityCommand::getEffectType()
{
	return EffectType::Entities;
}
