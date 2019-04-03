#include "Components/MovementComponent.h"


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
