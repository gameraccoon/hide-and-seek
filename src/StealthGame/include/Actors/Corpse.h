#pragma once

#include <Core/Actor.h>

class Corpse : public Actor
{
public:
	Corpse(World *world, Vector2D location, Rotator rotation);
	~Corpse() override;
};
