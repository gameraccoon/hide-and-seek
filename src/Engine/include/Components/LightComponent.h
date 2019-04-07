#pragma once

#include <memory>

#include "Core/Component.h"
#include "Components/TransformComponent.h"
#include "Core/Vector2D.h"

/**
 * Component that stores information about a light emitter
 */
class LightComponent : public BaseComponent
{
public:
	virtual ~LightComponent() override = default;

	float getBrightness() const;
	void setBrightness(float newSize);

	virtual void toJson(nlohmann::json& outJson) const override;

	friend void to_json(nlohmann::json& outJson, const LightComponent& light);
	friend void from_json(const nlohmann::json& json, LightComponent& outLight);

private:
	float mBrightness;
};
