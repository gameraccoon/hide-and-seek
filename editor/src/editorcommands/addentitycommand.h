#pragma once

#include "editorcommand.h"
#include "ECS/Entity.h"

class World;

class AddEntityCommand : public EditorCommand
{
public:
	AddEntityCommand(Entity entity);

	void doCommand(World* world) override;
	void undoCommand(World* world) override;

private:
	Entity mEntity;
};
