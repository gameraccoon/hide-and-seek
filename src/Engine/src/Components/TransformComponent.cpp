#include "Components/TransformComponent.h"


TransformComponent::TransformComponent()
	: TransformComponent(Vector2D(0.0f, 0.0f), Rotator(0.0f))
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

Rotator TransformComponent::getRotation() const
{
	return mRotation;
}

void TransformComponent::setRotation(const Rotator& newRotation)
{
	mRotation = newRotation;
}
