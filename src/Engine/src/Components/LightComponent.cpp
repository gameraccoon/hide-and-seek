#include "Components/LightComponent.h"

float LightComponent::getBrightness() const
{
	return mBrightness;
}

void LightComponent::setBrightness(float newBrightness)
{
	mBrightness = newBrightness;
}
