#ifndef CORPSE_H
#define CORPSE_H

#include <Engine/Actors/Actor.h>

class Corpse : public Actor
{
public:
	Corpse(World *world, Vector2D location, Vector2D scale, Rotator rotation);
	~Corpse(void);
};

#endif