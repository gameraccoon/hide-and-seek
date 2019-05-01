#pragma once

#include "Core/Component.h"

#include "Core/Vector2D.h"
#include "Core/Rotator.h"

/**
 * Component that stores information about position and movement
 */
class TransformComponent : public BaseComponent
{
public:
	Vector2D getLocation() const;
	void setLocation(const Vector2D& newLocation);

	Rotator getRotation() const;
	void setRotation(const Rotator& newRotation);

public:
	static std::string GetClassName() { return "TransformComponent"; }

	virtual void toJson(nlohmann::json& outJson) const override;
	virtual void fromJson(const nlohmann::json& json) override;

	friend void to_json(nlohmann::json& outJson, const TransformComponent& transform);
	friend void from_json(const nlohmann::json& json, TransformComponent& outTransform);

	std::string getComponentTypeName() const override { return GetClassName(); }

private:
	Vector2D mLocation = ZERO_VECTOR;
	Rotator mRotation = Rotator(0.0f);
};
