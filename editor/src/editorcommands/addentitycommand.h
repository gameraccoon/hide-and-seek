#pragma once

#include "editorcommand.h"
#include <Core/Entity.h>

class World;

class AddEntityCommand : public EditorCommand
{
public:
	AddEntityCommand(Entity mEntity);

	bool doCommand(World* world) override;
	bool undoCommand(World* world) override;

private:
	Entity mEntity;
};
