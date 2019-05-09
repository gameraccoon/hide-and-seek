#include "removeentitycommand.h"

#include <QtWidgets/qcombobox.h>

#include <Core/World.h>

#include "../mainwindow.h"


RemoveEntityCommand::RemoveEntityCommand(Entity entity)
	: mEntity(entity)
{
}

bool RemoveEntityCommand::doCommand(World* world)
{
	world->getEntityManger().removeEntity(mEntity);
	return false;
}

bool RemoveEntityCommand::undoCommand(World* /*world*/)
{
	// ToDo: implement
	assert(false);
}
