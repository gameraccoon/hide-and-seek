#pragma once

#include <memory>

#include "Core/Component.h"
#include "Core/Vector2D.h"

/**
 * Component that contains camera-relating information
 */
class CameraComponent : public BaseComponent
{
public:
	virtual ~CameraComponent() override = default;

	virtual void toJson(nlohmann::json& outJson) const override;

	friend void to_json(nlohmann::json& outJson, const CameraComponent& camera);
	friend void from_json(const nlohmann::json& json, CameraComponent& outCamera);
};
