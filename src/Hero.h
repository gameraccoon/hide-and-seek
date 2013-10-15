#ifndef HERO_H
#define HERO_H

#include "../src/Globals.h"
#include "../src/Vector2D.h"
#include "../src/DirectionArrow.h"

class Hero
{
public:
	/** Initialization of a new Hero standing at a given point */
	Hero(Vector2D location);

	~Hero();

	/** Set new location of the hero on the screen */
	void SetLocation(Vector2D newLocations);

	/** Say that we want to move the Hero on this step */
	void Move(Vector2D step);

	/**  */
	Vector2D GetLocation();

	/** Process moving and other actions of the Hero */
	void Update(float deltaTime);

	/** Render Hero in his current position on the screen */
	void Render();

private:
	/** Current Hero location on the screen */
	Vector2D Location;
	/** Hero moving speed in Px/s */
	float Speed;
	/** Delta between needless position and current position */
	Vector2D Step;
	/** Sprite of our Hero */
	hgeSprite* HeroSprite;
	/** Texture of all Hero sprites */
	HTEXTURE HeroTexture;
};

#endif