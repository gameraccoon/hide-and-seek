#pragma once

#include <string>

#include "EngineInterface/Internal/Engine.h"

#include <Engine/Core/Vector2D.h>
#include <Engine/Core/Rotator.h>

#include <EngineInterface/GUI/IGraphicElement.h>

class DirectionArrow : public IGraphicElement
{
public:
	DirectionArrow(Engine::Internal::Engine *engine);
	virtual ~DirectionArrow();
	/** Set center of arrow in world coordinates */
	virtual void setScreenLocation(const Vector2D& scrLocation) override;
	virtual Vector2D getScreenLocation() const override;
	/** Set new direction of arrow */
	void setDirection(Rotator direction);
	/** Set new direction by vector */
	void setVDirection(Vector2D vectDirection);
	/** Set color of the arrow */
	//void setColor(DWORD color);
	/** Render arrow to screen */
	virtual void render() const override;
	virtual bool click() override;
	virtual bool checkHovered(const Vector2D& mousePos) override;
private:
	/** Location of center of this arrow */
	Vector2D mCenterLocation;
	/** Arrow direction in world */
	Rotator mDirection;
	/** Pointer of the Engine subsystem */
	//Engine *engine;
	/** Is arrow can be rendered now? */
	bool mIsDrawable;
	//engineSprite* arrowSprite;
};
