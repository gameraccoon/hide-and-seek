#pragma once

#include "editorcommand.h"

#include <Core/Entity.h>
#include "Modules/ComponentFactory.h"

#include <nlohmann/json.hpp>

#include <QString>

class World;

class RemoveComponentCommand : public EditorCommand
{
public:
	RemoveComponentCommand(Entity mEntity, const QString& typeName, ComponentFactory* factory);

	bool doCommand(World* world) override;
	bool undoCommand(World* world) override;

private:
	Entity mEntity;
	QString mComponentTypeName;
	ComponentFactory* mComponentFactory;
	nlohmann::json mSerializedComponent;
};
