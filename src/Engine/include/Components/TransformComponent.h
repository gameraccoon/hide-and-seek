#pragma once

#include <functional>
#include <memory>

#include "Core/ActorComponent.h"

#include <Core/Vector2D.h>

/**
 * Component that stores informtaon about position and movement
 */
class TransformComponent : public ActorComponent
{
public:
	using Ptr = std::shared_ptr<TransformComponent>;
	using WeakPtr = std::weak_ptr<TransformComponent>;

public:
	TransformComponent();
	TransformComponent(const Vector2D& location, const Rotator& rotation);
	virtual ~TransformComponent() = default;

	Vector2D getLocation() const;
	void setLocation(const Vector2D& newLocation);

	Rotator getRotation() const;
	void setRotation(const Rotator& newRotation);

private:
	Vector2D mLocation;
	Rotator mRotation;
};
