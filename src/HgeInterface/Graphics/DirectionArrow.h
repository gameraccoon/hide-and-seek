#ifndef DIRECTIONARROW_H
#define DIRECTIONARROW_H

#include <string>

#include <hge.h>
#include <hgesprite.h>
#include <hgefont.h>
#include <hgeresource.h>

#include <Engine/Core/Vector2D.h>
#include <Engine/Core/Rotator.h>

#include <Engine/Subsystems/Graphic/GUI/IGraphicElement.h>

class DirectionArrow : public IGraphicElement
{
public:
	DirectionArrow(HGE *hge);
	virtual ~DirectionArrow(void);
	/** Set center of arrow in world coordinates */
	virtual void setScreenLocation(const Vector2D& scrLocation) override;
	virtual Vector2D getScreenLocation() const override;
	/** Set new direction of arrow */
	void setDirection(Rotator direction);
	/** Set new direction by vector */
	void setVDirection(Vector2D vectDirection);
	/** Set color of the arrow */
	void setColor(DWORD color);
	/** Render arrow to screen */
	virtual void render() const override;
	virtual bool click() override;
	virtual bool checkHovered(const Vector2D& mousePos) override;
private:
	/** Location of center of this arrow */
	Vector2D centerLocation;
	/** Arrow direction in world */
	Rotator direction;
	/** Pointer of the HGE subsystem */
	HGE *hge;
	/** Is arrow can be rendered now? */
	bool bDrawable;
	hgeSprite* arrowSprite;
};

#endif
