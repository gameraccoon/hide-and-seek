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
	StringID componentTypeNameID = static_cast<StringID>(mComponentTypeName.toStdString());
	world->getEntityManager().addComponent(
		mEntity,
		mComponentFactory->createComponent(componentTypeNameID),
		mComponentFactory->getTypeIDFromString(componentTypeNameID).value()
	);
	return false;
}

bool AddComponentCommand::undoCommand(World* world)
{
	world->getEntityManager().removeComponent(
		mEntity,
		mComponentFactory->getTypeIDFromString(static_cast<StringID>(mComponentTypeName.toStdString())).value()
	);
	return false;
}

EditorCommand::EffectType AddComponentCommand::getEffectType()
{
	return EffectType::Components;
}
