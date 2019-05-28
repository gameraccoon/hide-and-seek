#pragma once

#include "editorcommand.h"

#include "Core/Entity.h"
#include "Utils/Core/ComponentFactory.h"

#include <QString>

class World;

class AddComponentCommand : public EditorCommand
{
public:
	AddComponentCommand(Entity mEntity, const QString& typeName, ComponentFactory* factory);

	bool doCommand(World* world) override;
	bool undoCommand(World* world) override;
	EffectType getEffectType() override;

private:
	Entity mEntity;
	QString mComponentTypeName;
	ComponentFactory* mComponentFactory;
};
