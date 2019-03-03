#include "Components/LightComponent.h"

float LightComponent::getBrightness() const
{
	return mBrightness;
}

void LightComponent::setBrightness(float newBrightness)
{
	mBrightness = newBrightness;
}

MovementComponent::WeakPtr LightComponent::getMovementComponent() const
{
	return mMovementComponent;
}

void LightComponent::setMovementComponent(MovementComponent::WeakPtr newMovementComponent)
{
	mMovementComponent = newMovementComponent;
}
