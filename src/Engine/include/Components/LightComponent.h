#pragma once

#include <memory>

#include "Core/ActorComponent.h"
#include "Components/MovementComponent.h"
#include "Core/Vector2D.h"

/**
 *
 */
class LightComponent : public ActorComponent
{
public:
	using Ptr = std::shared_ptr<LightComponent>;
	using WeakPtr = std::weak_ptr<LightComponent>;

public:
	virtual ~LightComponent() = default;

	float getBrightness() const;
	void setBrightness(float newSize);

	MovementComponent::WeakPtr getMovementComponent() const;
	void setMovementComponent(MovementComponent::WeakPtr newMovementComponent);

private:
	float mBrightness;

	MovementComponent::WeakPtr mMovementComponent;
};
