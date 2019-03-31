#include "Components/RenderComponent.h"


RenderComponent::RenderComponent(const Graphics::Texture& texture)
	: mScale(1.0f, 1.0f)
	, mAnchor(0.5f, 0.5f)
	, mTexture(texture)
{
}

Vector2D RenderComponent::getScale() const
{
	return mScale;
}

void RenderComponent::setScale(const Vector2D& newScale)
{
	mScale = newScale;
}

Vector2D RenderComponent::getAnchor() const
{
	return mAnchor;
}

void RenderComponent::setAnchor(const Vector2D& newAnchor)
{
	mAnchor = newAnchor;
}

void RenderComponent::calcScaleFromSize(const Vector2D& size)
{
	mScale = Vector2D(size.x / mTexture.getWidth(), size.y / mTexture.getHeight());
}

const Graphics::Texture& RenderComponent::getTexture() const
{
	return mTexture;
}
