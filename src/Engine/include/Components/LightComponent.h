#pragma once

#include "Core/Component.h"

/**
 * Component that stores information about a light emitter
 */
class LightComponent : public BaseComponent
{
public:
	float getBrightness() const;
	void setBrightness(float newBrightness);

public:
	static std::string GetClassName() { return "LightComponent"; }

	virtual void toJson(nlohmann::json& outJson) const override;
	virtual void fromJson(const nlohmann::json& json) override;

	friend void to_json(nlohmann::json& outJson, const LightComponent& light);
	friend void from_json(const nlohmann::json& json, LightComponent& outLight);

	std::string getComponentTypeName() const override { return GetClassName(); }

private:
	float mBrightness = 1.0f;
};
