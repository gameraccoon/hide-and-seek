#include "addentitycommand.h"

#include <QtWidgets/qcombobox.h>

#include <GameData/World.h>

AddEntityCommand::AddEntityCommand(Entity entity)
	: mEntity(entity)
{
}

bool AddEntityCommand::doCommand(World* world)
{
	world->getEntityManager().insertEntityUnsafe(mEntity);
	return false;
}

bool AddEntityCommand::undoCommand(World* world)
{
	world->getEntityManager().removeEntity(mEntity);
	return false;
}

EditorCommand::EffectType AddEntityCommand::getEffectType()
{
	return EffectType::Entities;
}
