#pragma once

#include "EngineInterface/Internal/Engine.h"

#include <string>

#include "IGraphicElement.h"

class Button : public IGraphicElement
{
public:
	Button(Engine::Internal::Engine *engine, std::string spriteNormal, std::string spriteHover);
	virtual ~Button() = default;
	virtual void render() const override;
	virtual Vector2D getScreenLocation() const override;
	virtual void setScreenLocation(const Vector2D& scrLocation) override;
	/**
	 * @return false if we missed
	 */
	virtual bool click() override;

	virtual bool checkHovered(const Vector2D& mousePos) override;
private:
	//Engine *mEngine;
	
	Vector2D mScreenLocation;

	//engineSprite *mNormalSprite;
	//engineSprite *mHoverSprite;
};
