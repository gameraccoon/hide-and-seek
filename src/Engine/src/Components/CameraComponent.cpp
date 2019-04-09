#include "Components/CameraComponent.h"

#include <nlohmann/json.hpp>

void CameraComponent::toJson(nlohmann::json& outJson) const
{
	to_json(outJson, *this);
}

void CameraComponent::fromJson(const nlohmann::json& json)
{
	from_json(json, *this);
}

void to_json(nlohmann::json& outJson, const CameraComponent& /*camera*/)
{
	outJson = nlohmann::json{};
}

void from_json(const nlohmann::json& /*json*/, CameraComponent& /*outCamera*/)
{
}
