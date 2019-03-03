#pragma once

#include <memory>

#include "Core/ActorComponent.h"
#include "Components/MovementComponent.h"

/**
 *
 */
class RenderComponent : public ActorComponent
{
public:
	using Ptr = std::shared_ptr<RenderComponent>;
	using WeakPtr = std::weak_ptr<RenderComponent>;

public:
	RenderComponent();
	virtual ~RenderComponent() = default;

	MovementComponent::WeakPtr getMovementComponent() const;
	void setMovementComponent(MovementComponent::WeakPtr newMovementComponent);

	Vector2D getScale() const;
	void setScale(const Vector2D& newScale);

private:
	MovementComponent::WeakPtr mMovementComponent;

	Vector2D mScale;
};
