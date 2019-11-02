#include "addcomponentcommand.h"

#include <QtWidgets/qcombobox.h>

#include <GameData/World.h>

AddComponentCommand::AddComponentCommand(Entity entity, StringID typeName, ComponentFactory* factory)
	: mEntity(entity)
	, mComponentTypeName(typeName)
	, mComponentFactory(factory)
{
}

bool AddComponentCommand::doCommand(World* world)
{
	world->getEntityManager().addComponent(
		mEntity,
		mComponentFactory->createComponent(mComponentTypeName),
		mComponentFactory->getTypeIDFromString(mComponentTypeName).value()
	);
	return false;
}

bool AddComponentCommand::undoCommand(World* world)
{
	world->getEntityManager().removeComponent(
		mEntity,
		mComponentFactory->getTypeIDFromString(mComponentTypeName).value()
	);
	return false;
}

EditorCommand::EffectType AddComponentCommand::getEffectType()
{
	return EffectType::Components;
}
