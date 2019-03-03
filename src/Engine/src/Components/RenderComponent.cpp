#include "Components/RenderComponent.h"


RenderComponent::RenderComponent()
	: mScale(0.0f, 0.0f)
{
}

MovementComponent::WeakPtr RenderComponent::getMovementComponent() const
{
	return mMovementComponent;
}

void RenderComponent::setMovementComponent(MovementComponent::WeakPtr newMovementComponent)
{
	mMovementComponent = newMovementComponent;
}

Vector2D RenderComponent::getScale() const
{
	return mScale;
}

void RenderComponent::setScale(const Vector2D& newScale)
{
	mScale = newScale;
}
