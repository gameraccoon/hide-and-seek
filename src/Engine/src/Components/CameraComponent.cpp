#include "Components/CameraComponent.h"

TransformComponent::WeakPtr CameraComponent::getTransformComponent() const
{
	return mTransformComponent;
}

void CameraComponent::setTransformComponent(TransformComponent::WeakPtr newTransformComponent)
{
	mTransformComponent = newTransformComponent;
}
