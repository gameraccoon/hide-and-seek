#pragma once

#include <memory>

#include "Core/ActorComponent.h"
#include "Components/TransformComponent.h"
#include "Core/Vector2D.h"

/**
 * Component that contains camera-relating information
 */
class CameraComponent : public ActorComponent
{
public:
	using Ptr = std::shared_ptr<CameraComponent>;
	using WeakPtr = std::weak_ptr<CameraComponent>;

public:
	virtual ~CameraComponent() = default;

	TransformComponent::WeakPtr getTransformComponent() const;
	void setTransformComponent(TransformComponent::WeakPtr newTransformComponent);

private:
	TransformComponent::WeakPtr mTransformComponent;
};
