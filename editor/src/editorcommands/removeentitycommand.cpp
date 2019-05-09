#include "removeentitycommand.h"

#include <QtWidgets/qcombobox.h>

#include <Core/World.h>
#include <Debug/Assert.h>

RemoveEntityCommand::RemoveEntityCommand(Entity entity)
	: mEntity(entity)
{
}

bool RemoveEntityCommand::doCommand(World* world)
{
	world->getEntityManger().removeEntity(mEntity);
	return false;
}

bool RemoveEntityCommand::undoCommand(World* world)
{
	world->getEntityManger().insertEntityUnsafe(mEntity);
	Assert(false, "RemoveEntityCommand::undoCommand not implemented properly");
	return false;
}
