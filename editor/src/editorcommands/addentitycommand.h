#pragma once

#include "editorcommand.h"
#include "ECS/Entity.h"

class World;

class AddEntityCommand : public EditorCommand
{
public:
	AddEntityCommand(Entity mEntity);

	bool doCommand(World* world) override;
	bool undoCommand(World* world) override;
	EffectType getEffectType() override;

private:
	Entity mEntity;
};
