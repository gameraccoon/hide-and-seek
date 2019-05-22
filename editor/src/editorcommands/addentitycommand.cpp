#include "addentitycommand.h"

#include <QtWidgets/qcombobox.h>

#include <Core/World.h>

AddEntityCommand::AddEntityCommand(Entity entity)
	: mEntity(entity)
{
}

bool AddEntityCommand::doCommand(World* world)
{
	world->getEntityManger().insertEntityUnsafe(mEntity);
	return false;
}

bool AddEntityCommand::undoCommand(World* world)
{
	world->getEntityManger().removeEntity(mEntity);
	return false;
}

EditorCommand::EffectType AddEntityCommand::getEffectType()
{
	return EffectType::Entities;
}
