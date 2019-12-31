#pragma once

#include "editorcommand.h"
#include "GameData/Spatial/SpatialEntity.h"

class World;

class AddSpatialEntityCommand : public EditorCommand
{
public:
	AddSpatialEntityCommand(const SpatialEntity& entity, const Vector2D& location);

	bool doCommand(World* world) override;
	bool undoCommand(World* world) override;
	EffectType getEffectType() override;

private:
	SpatialEntity mEntity;
	Vector2D mLocation;
};
