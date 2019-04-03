#include "Components/TransformComponent.h"


TransformComponent::TransformComponent()
	: mLocation(0.0f, 0.0f)
	, mRotation(0.0f)
{
}

TransformComponent::TransformComponent(const Vector2D& location, const Rotator& rotation)
	: mLocation(location)
	, mRotation(rotation)
{
}

Vector2D TransformComponent::getLocation() const
{
	return mLocation;
}

void TransformComponent::setLocation(const Vector2D& newLocation)
{
	mLocation = newLocation;
}

void TransformComponent::shiftLocation(const Vector2D& deltaLocation)
{
	mLocation += deltaLocation;
}

Rotator TransformComponent::getRotation() const
{
	return mRotation;
}

void TransformComponent::setRotation(const Rotator& newRotation)
{
	mRotation = newRotation;
}

void TransformComponent::shiftRotation(const Rotator& deltaRotation)
{
	mRotation += deltaRotation;
}
