#pragma once

#include "editorcommand.h"

#include "GameData/Core/Vector2D.h"
#include "GameData/Spatial/SpatialEntity.h"

class World;

class ChangeEntityGroupLocationCommand : public EditorCommand
{
public:
	using SetterFunction = void (World::*)(const OptionalEntity&);

public:
	ChangeEntityGroupLocationCommand(const std::vector<SpatialEntity>& entities, const Vector2D& shift);

	void doCommand(World* world) override;
	void undoCommand(World* world) override;

private:
	std::vector<SpatialEntity> mEntities;
	std::vector<CellPos> mEntitiesCellPos;
	Vector2D mShift;
};
