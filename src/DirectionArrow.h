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
	DirectionArrow(void);
	~DirectionArrow(void);
	void SetCenter(Vector2D newCenter);
	void SetDirection(Rotator newDirection);
	void SetVDirection(Vector2D vectDirection);
	void Render();
private:
	Vector2D CenterLocation;
	/** Direction with length */
	Rotator Direction;
	bool bDrawable;
	hgeSprite* ArrowSprite;
	/** Texture includes all the sprites DirectionArrow */
	HTEXTURE ArrowTexture;
};

#endif