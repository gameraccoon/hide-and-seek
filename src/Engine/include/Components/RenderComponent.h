#pragma once

#include <memory>

#include "Core/Component.h"
#include "Core/Vector2D.h"
#include <Structures/ResourceHandle.h>

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

	std::string getTexturePath() const;
	void setTexturePath(const std::string& texturePath);

	Vector2D getSize() const;
	void setSize(const Vector2D& size);

	ResourceHandle getTextureHanle() const;
	void setTextureHanle(const ResourceHandle& textureHanle);

public:
	static std::string GetClassName() { return "RenderComponent"; }

	virtual void toJson(nlohmann::json& outJson) const override;
	virtual void fromJson(const nlohmann::json& json) override;

	friend void to_json(nlohmann::json& outJson, const RenderComponent& render);
	friend void from_json(const nlohmann::json& json, RenderComponent& outRender);

	std::string getComponentTypeName() const override { return GetClassName(); }

private:
	Vector2D mScale;
	Vector2D mSize;
	Vector2D mAnchor;
	std::string mTexturePath;
	ResourceHandle mTextureHanle;
};
