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

void RenderComponent::calcScaleFromSize(const Vector2D& size)
{
	if (mTexture && mTexture->isValid())
	{
		mScale = Vector2D(size.x / mTexture->getWidth(), size.y / mTexture->getHeight());
	}
}

const Graphics::Texture* RenderComponent::getTexture() const
{
	return mTexture.get();
}

void RenderComponent::setTexture(const Graphics::Texture& texture)
{
	mTexture = std::make_unique<Graphics::Texture>(texture);
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
//		{"texture", render.mTexture},
		{"scale", render.mScale},
		{"anchor", render.mAnchor}
	};
}

void from_json(const nlohmann::json& json, RenderComponent& outRender)
{
//	json.at("texture").get_to(outTransform.mTexture);
	json.at("scale").get_to(outRender.mScale);
	json.at("anchor").get_to(outRender.mAnchor);
}
