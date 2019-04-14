#include "Components/RenderComponent.h"

#include <nlohmann/json.hpp>

RenderComponent::RenderComponent()
	: mScale(0.0f, 0.0f)
	, mAnchor(0.5f, 0.5f)
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

void RenderComponent::calcScaleFromSize()
{
	if (mTexture.has_value() && mSize != ZERO_VECTOR)
	{
		if (const Graphics::Texture& texture = mTexture.value(); texture.isValid())
		{
			mScale = Vector2D(mSize.x / texture.getWidth(), mSize.y / texture.getHeight());
		}
	}
}

const std::optional<Graphics::Texture>& RenderComponent::getTexture() const
{
	return mTexture;
}

void RenderComponent::setTexture(const Graphics::Texture& texture)
{
	mTexture = texture;
}

void RenderComponent::toJson(nlohmann::json& outJson) const
{
	to_json(outJson, *this);
}

void RenderComponent::fromJson(const nlohmann::json& json)
{
	from_json(json, *this);
}

Vector2D RenderComponent::getSize() const
{
	return mSize;
}

void RenderComponent::setSize(const Vector2D& size)
{
	mSize = size;
}

std::string RenderComponent::getTexturePath() const
{
	return mTexturePath;
}

void RenderComponent::setTexturePath(const std::string& texturePath)
{
	mTexturePath = texturePath;
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
