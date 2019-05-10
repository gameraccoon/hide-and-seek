#pragma once

#include "editorcommand.h"
#include <Core/Entity.h>

#include <nlohmann/json.hpp>

class World;

class RemoveEntityCommand : public EditorCommand
{
public:
	RemoveEntityCommand(Entity entity, ComponentFactory* factory);

	bool doCommand(World* world) override;
	bool undoCommand(World* world) override;

private:
	Entity mEntity;
	ComponentFactory* mComponentFactory;
	nlohmann::json mSerializedComponents;
};
