#pragma once

#include <memory>

#include "Core/ActorComponent.h"
#include "Components/TransformComponent.h"

/**
 * Component that stores information about renderable part of an object
 */
class RenderComponent : public ActorComponent
{
public:
	using Ptr = std::shared_ptr<RenderComponent>;
	using WeakPtr = std::weak_ptr<RenderComponent>;

public:
	RenderComponent();
	virtual ~RenderComponent() = default;

	TransformComponent::WeakPtr getTransformComponent() const;
	void setTransformComponent(TransformComponent::WeakPtr newTransformComponent);

	Vector2D getScale() const;
	void setScale(const Vector2D& newScale);

	void setTexturePath(const std::string& newTexturePath);
	std::string getTexturePath();

private:
	TransformComponent::WeakPtr mTransformComponent;

	Vector2D mScale;

	// just for now
	std::string mTexturePath;
};
