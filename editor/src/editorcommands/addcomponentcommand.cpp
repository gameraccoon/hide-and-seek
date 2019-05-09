#include "addcomponentcommand.h"

#include <QtWidgets/qcombobox.h>

#include <Core/World.h>

#include "../mainwindow.h"


AddComponentCommand::AddComponentCommand(Entity entity, const QString& typeName, ComponentFactory* factory)
	: mEntity(entity)
	, mComponentTypeName(typeName)
	, mComponentFactory(factory)
{
}

bool AddComponentCommand::doCommand(World* world)
{
	world->getEntityManger().addComponent(
		mEntity,
		mComponentFactory->createComponent(mComponentTypeName.toStdString()),
		mComponentFactory->getTypeIDFromString(mComponentTypeName.toStdString()).value()
	);
	return false;
}

bool AddComponentCommand::undoCommand(World* world)
{
	world->getEntityManger().removeComponent(
		mEntity,
		mComponentFactory->getTypeIDFromString(mComponentTypeName.toStdString()).value()
	);
	return false;
}
