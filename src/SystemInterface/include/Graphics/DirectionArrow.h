#pragma once

#include <string>

#include <Core/Vector2D.h>
#include <Core/Rotator.h>

#include <EngineFwd.h>
#include <Graphics/GUI/IGraphicElement.h>

class DirectionArrow : public IGraphicElement
{
public:
	DirectionArrow(SystemInterface::Engine *engine);
	virtual ~DirectionArrow() override;
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
