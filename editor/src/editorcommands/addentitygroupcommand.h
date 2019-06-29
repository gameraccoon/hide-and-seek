#pragma once

#include <vector>

#include <nlohmann/json.hpp>

#include "editorcommand.h"

#include "ECS/Entity.h"
#include "GameData/Core/Vector2D.h"

class World;

class AddEntityGroupCommand : public EditorCommand
{
public:
	AddEntityGroupCommand(const std::vector<nlohmann::json>& entities, ComponentFactory* factory, const Vector2D& shift);

	bool doCommand(World* world) override;
	bool undoCommand(World* world) override;
	EffectType getEffectType() override;

private:
	std::vector<nlohmann::json> mEntities;
	std::vector<Entity> mCreatedEntities;
	ComponentFactory* mComponentFactory;
	Vector2D mShift;
};
