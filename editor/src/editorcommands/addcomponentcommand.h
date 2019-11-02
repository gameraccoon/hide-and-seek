#pragma once

#include "editorcommand.h"

#include "ECS/Entity.h"
#include "ECS/ComponentFactory.h"

class World;

class AddComponentCommand : public EditorCommand
{
public:
	AddComponentCommand(Entity mEntity, StringID typeName, ComponentFactory* factory);

	bool doCommand(World* world) override;
	bool undoCommand(World* world) override;
	EffectType getEffectType() override;

private:
	Entity mEntity;
	StringID mComponentTypeName;
	ComponentFactory* mComponentFactory;
};
