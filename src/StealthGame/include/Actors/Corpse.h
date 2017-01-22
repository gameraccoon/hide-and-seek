#pragma once

#include <Actors/Actor.h>

class Corpse : public Actor
{
public:
	Corpse(World *world, Vector2D location, Vector2D scale, Rotator rotation);
	~Corpse();
};
