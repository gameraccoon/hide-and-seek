#pragma once

#include "editorcommand.h"

#include <Core/Entity.h>
#include <Core/Vector2D.h>

class World;

class ChangeEntityGroupLocationCommand : public EditorCommand
{
public:
	typedef void (World::*SetterFunction)(const NullableEntity&);

public:
	ChangeEntityGroupLocationCommand(const std::vector<Entity>& entities, const Vector2D& shift);

	bool doCommand(World* world) override;
	bool undoCommand(World* world) override;

private:
	std::vector<Entity> mEntities;
	Vector2D mShift;
};
