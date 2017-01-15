#pragma once

#include <Engine/Actors/Actor.h>

class Wall : public Actor
{
public:
	Wall(World *world, Vector2D location, Vector2D scale, Rotator rotation);
	~Wall();
	/** Dummy (wall do nothing) */
	virtual void update(float deltatime) override;
};
