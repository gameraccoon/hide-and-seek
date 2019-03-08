#include "Components/LightComponent.h"

float LightComponent::getBrightness() const
{
	return mBrightness;
}

void LightComponent::setBrightness(float newBrightness)
{
	mBrightness = newBrightness;
}

TransformComponent::WeakPtr LightComponent::getTransformComponent() const
{
	return mTransformComponent;
}

void LightComponent::setTransformComponent(TransformComponent::WeakPtr newTransformComponent)
{
	mTransformComponent = newTransformComponent;
}
