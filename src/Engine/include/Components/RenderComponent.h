#pragma once

#include <memory>

#include "Core/Component.h"
#include "Core/Vector2D.h"
#include <Graphics/Texture.h>

/**
 * Component that stores information about renderable part of an object
 */
class RenderComponent : public BaseComponent
{
public:
	using Ptr = std::shared_ptr<RenderComponent>;
	using WeakPtr = std::weak_ptr<RenderComponent>;

public:
	RenderComponent(const Graphics::Texture& texture);
	virtual ~RenderComponent() override = default;

	Vector2D getScale() const;
	void setScale(const Vector2D& newScale);

	Vector2D getAnchor() const;
	void setAnchor(const Vector2D& newAnchor);

	void setTexturePath(const std::string& newTexturePath);
	std::string getTexturePath();

	const Graphics::Texture& getTexture() const;

private:
	Vector2D mScale;
	Vector2D mAnchor;
	Graphics::Texture mTexture;
};
