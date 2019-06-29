#pragma once

#include "editorcommand.h"

#include "ECS/Entity.h"

#include <nlohmann/json.hpp>

#include <QString>

class World;
class ComponentFactory;

class RemoveComponentCommand : public EditorCommand
{
public:
	RemoveComponentCommand(Entity mEntity, const QString& typeName, ComponentFactory* factory);

	bool doCommand(World* world) override;
	bool undoCommand(World* world) override;
	EffectType getEffectType() override;

private:
	Entity mEntity;
	QString mComponentTypeName;
	ComponentFactory* mComponentFactory;
	nlohmann::json mSerializedComponent;
};
