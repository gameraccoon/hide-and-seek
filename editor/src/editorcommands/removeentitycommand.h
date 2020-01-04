#pragma once

#include "editorcommand.h"
#include "ECS/Entity.h"

#include <nlohmann/json.hpp>

class World;

class RemoveEntityCommand : public EditorCommand
{
public:
	RemoveEntityCommand(Entity entity, ComponentFactory* factory);

	void doCommand(World* world) override;
	void undoCommand(World* world) override;

private:
	Entity mEntity;
	ComponentFactory* mComponentFactory;
	nlohmann::json mSerializedComponents;
};
