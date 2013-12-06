#ifndef HERO_H
#define HERO_H

#include "../src/Globals.h"
#include "../src/World.h"
#include "../src/Vector2D.h"
#include "../src/Man.h"

class Hero:public Man
{
public:
	/** Initialization of a new Hero standing at a given point */
	Hero(World *ownerWorld, Vector2D location);

	~Hero();
};

#endif