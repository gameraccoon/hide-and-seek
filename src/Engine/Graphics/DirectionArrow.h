#ifndef DIRECTIONARROW_H
#define DIRECTIONARROW_H

#include "../Hge/HgeIncludes.h"
#include "../Core/Vector2D.h"
#include "../Core/Rotator.h"

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
	void setCenter(Vector2D center);
	/** Set new direction of arrow */
	void setDirection(Rotator direction);
	/** Set new direction by vector */
	void setVDirection(Vector2D vectDirection);
	/** Render arrow to screen */
	void render();
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
