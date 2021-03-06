#pragma once

#include <vector>

#include <nlohmann/json.hpp>

#include "editorcommand.h"

#include "GameData/Core/Vector2D.h"
#include "GameData/Spatial/SpatialEntity.h"

class World;

class AddEntityGroupCommand : public EditorCommand
{
public:
	AddEntityGroupCommand(const std::vector<nlohmann::json>& entities, const ComponentSerializersHolder& serializationHolder, const Vector2D& shift);

	void doCommand(World* world) override;
	void undoCommand(World* world) override;

private:
	std::vector<nlohmann::json> mEntities;
	std::vector<SpatialEntity> mCreatedEntities;
	const ComponentSerializersHolder& mSerializationHolder;
	Vector2D mShift;
};
