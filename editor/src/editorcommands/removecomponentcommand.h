#pragma once

#include "editorcommand.h"

#include "ECS/Entity.h"

#include <nlohmann/json.hpp>

#include "src/editorutils/componentreference.h"

class World;
class ComponentSerializersHolder;

class RemoveComponentCommand : public EditorCommand
{
public:
	RemoveComponentCommand(const ComponentSourceReference& source, StringID typeName, const ComponentSerializersHolder& serializerHolder);

	void doCommand(World* world) override;
	void undoCommand(World* world) override;

private:
	ComponentSourceReference mSource;
	StringID mComponentTypeName;
	const ComponentSerializersHolder& mComponentSerializerHolder;
	nlohmann::json mSerializedComponent;
};
