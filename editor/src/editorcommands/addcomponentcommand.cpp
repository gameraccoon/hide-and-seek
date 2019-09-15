#include "addcomponentcommand.h"

#include <QtWidgets/qcombobox.h>

#include <GameData/World.h>

AddComponentCommand::AddComponentCommand(Entity entity, const QString& typeName, ComponentFactory* factory)
	: mEntity(entity)
	, mComponentTypeName(typeName)
	, mComponentFactory(factory)
{
}

bool AddComponentCommand::doCommand(World* world)
{
	world->getEntityManager().addComponent(
		mEntity,
		mComponentFactory->createComponent(mComponentTypeName.toStdString()),
		mComponentFactory->getTypeIDFromString(mComponentTypeName.toStdString()).value()
	);
	return false;
}

bool AddComponentCommand::undoCommand(World* world)
{
	world->getEntityManager().removeComponent(
		mEntity,
		mComponentFactory->getTypeIDFromString(mComponentTypeName.toStdString()).value()
	);
	return false;
}

EditorCommand::EffectType AddComponentCommand::getEffectType()
{
	return EffectType::Components;
}
