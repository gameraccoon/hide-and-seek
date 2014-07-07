#ifndef DIRECTIONARROW_H
#define DIRECTIONARROW_H

#include <string>

#include <hge.h>
#include <hgesprite.h>
#include <hgefont.h>
#include <hgeresource.h>

#include <Engine/Core/Vector2D.h>
#include <Engine/Core/Rotator.h>

class DirectionArrow
{
public:
	DirectionArrow(HGE *hge);
	~DirectionArrow(void);
	/** Set center of arrow in world coordinates */
	void setCenter(Vector2D center);
	/** Set new direction of arrow */
	void setDirection(Rotator direction);
	/** Set new direction by vector */
	void setVDirection(Vector2D vectDirection);
	/** Set color of the arrow */
	void setColor(DWORD color);
	/** Render arrow to screen */
	void render() const;
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
	/** Texture includes all the sprites DirectionArrow */
	HTEXTURE arrowTexture;
};

#endif
