#pragma once

#include <functional>
#include <memory>

#include "Core/ActorComponent.h"

#include <Core/Vector2D.h>

/**
 * Component that stores informtaon and logic about position and movement
 */
class MovementComponent : public ActorComponent
{
public:
	using Ptr = std::shared_ptr<MovementComponent>;
	using WeakPtr = std::weak_ptr<MovementComponent>;

public:
	MovementComponent();
	MovementComponent(const Vector2D& location, const Rotator& rotation);
	virtual ~MovementComponent() = default;

	Vector2D getLocation() const;
	void setLocation(const Vector2D& newLocation);

	Rotator getRotation() const;
	void setRotation(const Rotator& newRotation);

	void overrideOnUpdateLocationCallback(std::function<void()> newCallback);
	void overrideOnUpdateRotationCallback(std::function<void()> newCallback);
private:
	Vector2D mLocation;
	Rotator mRotation;

	std::function<void()> mOnUpdateLocation;
	std::function<void()> mOnUpdateRotation;
};
