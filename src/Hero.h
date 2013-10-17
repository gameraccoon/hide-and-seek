#ifndef HERO_H
#define HERO_H

#include "../src/Globals.h"
#include "../src/Vector2D.h"
#include "../src/DirectionArrow.h"
#include "../src/Actor.h"

class Hero:public Actor
{
public:
	/** Initialization of a new Hero standing at a given point */
	Hero(Vector2D location);

	~Hero();

	/** Say that we want to move the Hero on this step */
	void Move(Vector2D step);

	/** Process moving and other actions of the Hero */
	void Update(float deltaTime);
	/** Render the actor in the current location */
	void Render(Vector2D shift);

private:
	/** Hero moving speed in Px/s */
	float Speed;
	/** Delta between needless position and current position */
	Vector2D Step;
	/** Texture of all Hero sprites */
	HTEXTURE HeroTexture;
};

#endif