#include "Components/MovementComponent.h"

#include <nlohmann/json.hpp>

Vector2D MovementComponent::getSpeed() const
{
	return mSpeed;
}

void MovementComponent::setSpeed(const Vector2D& newSpeed)
{
	mSpeed = newSpeed;
}

Vector2D MovementComponent::getLastStep() const
{
	return mLastStep;
}

void MovementComponent::setLastStep(const Vector2D& newLastStep)
{
	mLastStep = newLastStep;
}

void MovementComponent::toJson(nlohmann::json& outJson) const
{
	to_json(outJson, *this);
}

void MovementComponent::fromJson(const nlohmann::json& json)
{
	from_json(json, *this);
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
