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
		world->getEntityManger().getPrefabFromEntity(mSerializedComponents, mEntity);
	}

	world->getEntityManger().removeEntity(mEntity);
	return false;
}

bool RemoveEntityCommand::undoCommand(World* world)
{
	world->getEntityManger().insertEntityUnsafe(mEntity);
	world->getEntityManger().applyPrefabToExistentEntity(mSerializedComponents, mEntity, *mComponentFactory);
	return false;
}

EditorCommand::EffectType RemoveEntityCommand::getEffectType()
{
	return EffectType::Entities;
}
