#pragma once

#include "editorcommand.h"
#include "ECS/Entity.h"

#include <nlohmann/json.hpp>

class World;

class RemoveEntityCommand : public EditorCommand
{
public:
	RemoveEntityCommand(Entity entity, ComponentFactory* factory);

	bool doCommand(World* world) override;
	bool undoCommand(World* world) override;
	EffectType getEffectType() override;

private:
	Entity mEntity;
	ComponentFactory* mComponentFactory;
	nlohmann::json mSerializedComponents;
};
