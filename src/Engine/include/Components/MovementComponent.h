#pragma once

#include <functional>
#include <memory>

#include "Core/Component.h"

#include <Core/Vector2D.h>

/**
 * Component that stores informtaon about position and movement
 */
class MovementComponent : public BaseComponent
{
public:
	MovementComponent();
	virtual ~MovementComponent() override = default;

	Vector2D getSpeed() const;
	void setSpeed(const Vector2D& speed);

	Vector2D getLastStep() const;
	void setLastStep(const Vector2D& lastStep);

public:
	static std::string GetClassName() { return "MovementComponent"; }

	virtual void toJson(nlohmann::json& outJson) const override;
	virtual void fromJson(const nlohmann::json& json) override;

	friend void to_json(nlohmann::json& outJson, const MovementComponent& movement);
	friend void from_json(const nlohmann::json& json, MovementComponent& outMovement);

private:
	Vector2D mSpeed;
	Vector2D mLastStep;
};
