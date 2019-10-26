#pragma once

#include "editorcommand.h"

#include "ECS/Entity.h"
#include "GameData/Core/Vector2D.h"

class World;

class ChangeEntityGroupLocationCommand : public EditorCommand
{
public:
	using SetterFunction = void (World::*)(const OptionalEntity&);

public:
	ChangeEntityGroupLocationCommand(const std::vector<Entity>& entities, const Vector2D& shift);

	bool doCommand(World* world) override;
	bool undoCommand(World* world) override;
	EffectType getEffectType() override;

private:
	std::vector<Entity> mEntities;
	Vector2D mShift;
};
