#ifndef HERO_H
#define HERO_H

#include "../../Engine/Core/World.h"
#include "../../Engine/Core/Vector2D.h"
#include "../../Engine/Actors/Body.h"

class Hero:public Body
{
public:
	/** Initialization of a new Hero standing at a given point */
	Hero(World *ownerWorld, Vector2D location);

	~Hero();

	/** Process moving and other actions of the man */
	virtual void update(float deltatime);
	
	/** Say that we want to move the man on this step */
	virtual void move(Vector2D step);

protected:
	/** Delta between needless position and current position */
	Vector2D step;
};

#endif
