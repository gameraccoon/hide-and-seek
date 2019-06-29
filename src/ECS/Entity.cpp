#include "Entity.h"

#include <nlohmann/json.hpp>

#include <Debug/Assert.h>

void to_json(nlohmann::json& outJson, const Entity& entity)
{
	outJson = nlohmann::json{{"id", entity.mId}};
}

void from_json(const nlohmann::json& json, Entity& outEntity)
{
	json.at("id").get_to(outEntity.mId);
}

Entity NullableEntity::getEntity() const {
	Assert(mIsValid, "Getting uninitialized entity");
	return Entity(mId);
}

void to_json(nlohmann::json& outJson, const NullableEntity& entity)
{
	outJson = nlohmann::json{
		{"valid", entity.mIsValid},
		{"id", entity.mId}
	};
}

void from_json(const nlohmann::json& json, NullableEntity& outEntity)
{
	json.at("valid").get_to(outEntity.mIsValid);
	json.at("id").get_to(outEntity.mId);
}
