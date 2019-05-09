#include "removecomponentcommand.h"

#include <QtWidgets/qcombobox.h>

#include <Core/World.h>
#include <Debug/Assert.h>

RemoveComponentCommand::RemoveComponentCommand(Entity entity, const QString& typeName, ComponentFactory* factory)
	: mEntity(entity)
	, mComponentTypeName(typeName)
	, mComponentFactory(factory)
{
}

bool RemoveComponentCommand::doCommand(World* world)
{
	world->getEntityManger().removeComponent(
		mEntity,
		mComponentFactory->getTypeIDFromString(mComponentTypeName.toStdString()).value()
	);
	return false;
}

bool RemoveComponentCommand::undoCommand(World* world)
{
	world->getEntityManger().addComponent(
		mEntity,
		mComponentFactory->createComponent(mComponentTypeName.toStdString()),
		mComponentFactory->getTypeIDFromString(mComponentTypeName.toStdString()).value()
	);
	Assert(false, "RemoveComponentCommand::undoCommand not implemented properly");
	return false;
}
