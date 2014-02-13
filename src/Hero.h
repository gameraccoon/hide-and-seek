#ifndef HERO_H
#define HERO_H

#include "../src/Globals.h"
#include "../src/World.h"
#include "../src/Vector2D.h"
#include "../src/Man.h"

class Hero:public DummyMan
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
