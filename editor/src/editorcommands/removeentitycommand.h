#pragma once

#include "editorcommand.h"
#include <Core/Entity.h>

class World;

class RemoveEntityCommand : public EditorCommand
{
public:
	RemoveEntityCommand(Entity entity);

	bool doCommand(World* world) override;
	bool undoCommand(World* world) override;

private:
	Entity mEntity;
};
