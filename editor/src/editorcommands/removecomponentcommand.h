#pragma once

#include "editorcommand.h"

#include "ECS/Entity.h"

#include <nlohmann/json.hpp>

#include "src/editorutils/componentreference.h"

class World;
class ComponentFactory;

class RemoveComponentCommand : public EditorCommand
{
public:
	RemoveComponentCommand(const ComponentSourceReference& source, StringID typeName, ComponentFactory* factory);

	void doCommand(World* world) override;
	void undoCommand(World* world) override;

private:
	ComponentSourceReference mSource;
	StringID mComponentTypeName;
	ComponentFactory* mComponentFactory;
	nlohmann::json mSerializedComponent;
};
