#ifndef DIRECTIONARROW_H
#define DIRECTIONARROW_H

#include "../src/Globals.h"
#include "../src/Vector2D.h"

// dummy for debugging methods
#if (!defined DEBUG) && (!defined RELEASE)
	#define RELEASE
	#define WARN(message)
	#define WARN_IF(condition, message)
#endif

class DirectionArrow
{
public:
	DirectionArrow(HGE *hge);
	~DirectionArrow(void);
	/** Set center of arrow in world coordinates */
	void SetCenter(Vector2D newCenter);
	/** Set new direction of arrow */
	void SetDirection(Rotator newDirection);
	/** Set new direction by vector */
	void SetVDirection(Vector2D vectDirection);
	/** Render arrow to screen */
	void Render();
private:
	/** Location of center of this arrow */
	Vector2D CenterLocation;
	/** Arrow direction in world */
	Rotator Direction;
	/** Pointer of the HGE subsystem */
	HGE *Hge;
	/** Is arrow can be rendered now? */
	bool bDrawable;
	hgeSprite* ArrowSprite;
	/** Texture includes all the sprites DirectionArrow */
	HTEXTURE ArrowTexture;
};

#endif