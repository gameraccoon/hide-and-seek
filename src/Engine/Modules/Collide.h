#ifndef COLLIDE_H
#define COLLIDE_H

#include <stdio.h>

#include "../Core/IActor.h"
#include "../Core/World.h"

namespace Collide
{
	bool isWillCollide(const IActor* actor1, const World* world, Vector2D step);
};

#endif
