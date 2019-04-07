#pragma once

#include <functional>
#include <memory>

#include "Core/Component.h"

#include <Core/Vector2D.h>

#include <nlohmann/json_fwd.hpp>

/**
 * Component that stores informtaon about position and movement
 */
class TransformComponent : public BaseComponent
{
public:
	TransformComponent();
	TransformComponent(const Vector2D& location, const Rotator& rotation);
	virtual ~TransformComponent() override = default;

	Vector2D getLocation() const;
	void setLocation(const Vector2D& newLocation);
	void shiftLocation(const Vector2D& deltaLocation);

	Rotator getRotation() const;
	void setRotation(const Rotator& newRotation);
	void shiftRotation(const Rotator& deltaRotation);

	virtual void toJson(nlohmann::json& outJson) const override;

	friend void to_json(nlohmann::json& outJson, const TransformComponent& transform);
	friend void from_json(const nlohmann::json& json, TransformComponent& outTransform);

private:
	Vector2D mLocation;
	Rotator mRotation;
};
