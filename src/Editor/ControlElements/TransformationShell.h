#ifndef TRANSFORMATION_SHELL_H
#define TRANSFORMATION_SHELL_H

#include <hge.h>
#include <hgesprite.h>
#include <hgefont.h>
#include <hgeresource.h>

#include <Engine/Core/Vector2D.h>
#include <Engine/Core/Rotator.h>

#include <Engine/Graphic/IGraphicElement.h>

class TransformationShell : public IGraphicElement
{
public:
	TransformationShell(HGE *hge);
	virtual ~TransformationShell(void);
	/** Set center of arrow in world coordinates */
	virtual void setScreenLocation(const Vector2D& scrLocation) override;
	virtual Vector2D getScreenLocation() const override;
	/** Render arrow to screen */
	virtual void render() const override;
	virtual bool click() override;
private:
	/** Pointer of the HGE subsystem */
	HGE *hge;
	hgeSprite* sprite;
	/** Texture includes all the sprites DirectionArrow */
	HTEXTURE texture;
	Vector2D screenLocation;
};

#endif
