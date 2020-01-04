#pragma once

#include "editorcommand.h"

#include "ECS/Entity.h"
#include "ECS/ComponentFactory.h"

#include "src/editorutils/componentreference.h"

class World;

class AddComponentCommand : public EditorCommand
{
public:
	AddComponentCommand(const ComponentSourceReference& source, StringID typeName, ComponentFactory* factory);

	void doCommand(World* world) override;
	void undoCommand(World* world) override;

private:
	ComponentSourceReference mSource;
	StringID mComponentTypeName;
	ComponentFactory* mComponentFactory;
};
