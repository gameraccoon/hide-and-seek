#pragma once

#include "Core/Component.h"

#include "Core/Vector2D.h"

/**
 * Component that stores information about position and movement
 */
class MovementComponent : public BaseComponent
{
public:
	Vector2D getSpeed() const;
	void setSpeed(const Vector2D& newSpeed);

	Vector2D getLastStep() const;
	void setLastStep(const Vector2D& newLastStep);

public:
	static std::string GetClassName() { return "MovementComponent"; }

	virtual void toJson(nlohmann::json& outJson) const override;
	virtual void fromJson(const nlohmann::json& json) override;

	friend void to_json(nlohmann::json& outJson, const MovementComponent& movement);
	friend void from_json(const nlohmann::json& json, MovementComponent& outMovement);

	std::string getComponentTypeName() const override { return GetClassName(); }

private:
	Vector2D mSpeed = ZERO_VECTOR;
	Vector2D mLastStep = ZERO_VECTOR;
};
