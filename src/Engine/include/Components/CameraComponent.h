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

public:
	static std::string GetClassName() { return "CameraComponent"; }

	virtual void toJson(nlohmann::json& outJson) const override;
	virtual void fromJson(const nlohmann::json& json) override;

	friend void to_json(nlohmann::json& outJson, const CameraComponent& camera);
	friend void from_json(const nlohmann::json& json, CameraComponent& outCamera);
};
