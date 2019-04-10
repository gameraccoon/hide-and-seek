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
	RenderComponent();

	Vector2D getScale() const;
	void setScale(const Vector2D& newScale);

	Vector2D getAnchor() const;
	void setAnchor(const Vector2D& newAnchor);

	void calcScaleFromSize(const Vector2D& size);

	const Graphics::Texture* getTexture() const;
	void setTexture(const Graphics::Texture& texture);

	static std::string GetClassName() { return "RenderComponent"; }

	virtual void toJson(nlohmann::json& outJson) const override;
	virtual void fromJson(const nlohmann::json& json) override;

	friend void to_json(nlohmann::json& outJson, const RenderComponent& render);
	friend void from_json(const nlohmann::json& json, RenderComponent& outRender);

private:
	Vector2D mScale;
	Vector2D mAnchor;
	std::unique_ptr<Graphics::Texture> mTexture;
};
