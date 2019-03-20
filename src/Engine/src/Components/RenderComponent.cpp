#include "Components/RenderComponent.h"


RenderComponent::RenderComponent()
	: mScale(0.0f, 0.0f)
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

void RenderComponent::setTexturePath(const std::string& newTexturePath)
{
	mTexturePath = newTexturePath;
}

std::string RenderComponent::getTexturePath()
{
	return mTexturePath;
}
