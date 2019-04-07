#include "Components/MovementComponent.h"

#include <nlohmann/json.hpp>

MovementComponent::MovementComponent()
	: mSpeed(ZERO_VECTOR)
	, mLastStep(ZERO_VECTOR)
{
}

Vector2D MovementComponent::getSpeed() const
{
    return mSpeed;
}

void MovementComponent::setSpeed(const Vector2D& speed)
{
    mSpeed = speed;
}

Vector2D MovementComponent::getLastStep() const
{
	return mLastStep;
}

void MovementComponent::setLastStep(const Vector2D& lastStep)
{
	mLastStep = lastStep;
}

void MovementComponent::toJson(nlohmann::json& outJson) const
{
	to_json(outJson, *this);
}

void to_json(nlohmann::json& outJson, const MovementComponent& movement)
{
	outJson = nlohmann::json{
		{"last_step", movement.mLastStep},
		{"speed", movement.mSpeed}
	};
}

void from_json(const nlohmann::json& json, MovementComponent& outMovement)
{
	json.at("last_step").get_to(outMovement.mLastStep);
	json.at("speed").get_to(outMovement.mSpeed);
}
