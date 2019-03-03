#include "Components/MovementComponent.h"


MovementComponent::MovementComponent()
	: MovementComponent(Vector2D(0.0f, 0.0f), Rotator(0.0f))
{
}

MovementComponent::MovementComponent(const Vector2D& location, const Rotator& rotation)
	: mLocation(location)
	, mRotation(rotation)
{
}

Vector2D MovementComponent::getLocation() const
{
	return mLocation;
}

void MovementComponent::setLocation(const Vector2D& newLocation)
{
	mLocation = newLocation;

	if (mOnUpdateLocation != nullptr)
	{
		mOnUpdateLocation();
	}
}

Rotator MovementComponent::getRotation() const
{
	return mRotation;
}

void MovementComponent::setRotation(const Rotator& newRotation)
{
	mRotation = newRotation;

	if (mOnUpdateRotation != nullptr)
	{
		mOnUpdateRotation();
	}
}

void MovementComponent::overrideOnUpdateLocationCallback(std::function<void ()> newCallback)
{
	mOnUpdateLocation = newCallback;
}

void MovementComponent::overrideOnUpdateRotationCallback(std::function<void ()> newCallback)
{
	mOnUpdateRotation = newCallback;
}
