#include "Components/LightComponent.h"

#include <nlohmann/json.hpp>

float LightComponent::getBrightness() const
{
	return mBrightness;
}

void LightComponent::setBrightness(float newBrightness)
{
	mBrightness = newBrightness;
}

void LightComponent::toJson(nlohmann::json& outJson) const
{
	to_json(outJson, *this);
}

void to_json(nlohmann::json& outJson, const LightComponent& light)
{
	outJson = nlohmann::json{
		{"brightness", light.mBrightness}
	};
}

void from_json(const nlohmann::json& json, LightComponent& outLight)
{
	json.at("brightness").get_to(outLight.mBrightness);
}
