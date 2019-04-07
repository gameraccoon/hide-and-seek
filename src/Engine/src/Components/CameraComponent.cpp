#include "Components/CameraComponent.h"

#include <nlohmann/json.hpp>

void CameraComponent::toJson(nlohmann::json& outJson) const
{
	to_json(outJson, *this);
}

void to_json(nlohmann::json& outJson, const CameraComponent& /*camera*/)
{
	outJson = nlohmann::json{};
}

void from_json(const nlohmann::json& /*json*/, CameraComponent& /*outCamera*/)
{
}
