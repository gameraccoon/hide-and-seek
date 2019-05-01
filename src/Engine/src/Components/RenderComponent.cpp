#include "Components/RenderComponent.h"

#include <nlohmann/json.hpp>

Vector2D RenderComponent::getScale() const
{
	return mScale;
}

void RenderComponent::setScale(const Vector2D& newScale)
{
	mScale = newScale;
}

Vector2D RenderComponent::getSize() const
{
	return mSize;
}

void RenderComponent::setSize(const Vector2D& newSize)
{
	mSize = newSize;
}

Vector2D RenderComponent::getAnchor() const
{
	return mAnchor;
}

void RenderComponent::setAnchor(const Vector2D& newAnchor)
{
	mAnchor = newAnchor;
}

std::string RenderComponent::getTexturePath() const
{
	return mTexturePath;
}

void RenderComponent::setTexturePath(const std::string& newTexturePath)
{
	mTexturePath = newTexturePath;
}

ResourceHandle RenderComponent::getTextureHandle() const
{
	return mTextureHandle;
}

void RenderComponent::setTextureHandle(const ResourceHandle& newTextureHandle)
{
	mTextureHandle = newTextureHandle;
}

void RenderComponent::toJson(nlohmann::json& outJson) const
{
	to_json(outJson, *this);
}

void RenderComponent::fromJson(const nlohmann::json& json)
{
	from_json(json, *this);
}

void to_json(nlohmann::json& outJson, const RenderComponent& render)
{
	outJson = nlohmann::json{
		{"texture", render.mTexturePath},
		{"size", render.mSize},
		{"scale", render.mScale},
		{"anchor", render.mAnchor}
	};
}

void from_json(const nlohmann::json& json, RenderComponent& outRender)
{
	json.at("texture").get_to(outRender.mTexturePath);
	json.at("size").get_to(outRender.mSize);
	json.at("scale").get_to(outRender.mScale);
	json.at("anchor").get_to(outRender.mAnchor);
}
