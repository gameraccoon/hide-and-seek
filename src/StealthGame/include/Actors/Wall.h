#pragma once

#include <Core/Actor.h>

class Wall : public Actor
{
public:
	Wall(World *world, const Vector2D location, const Vector2D scale, const Rotator rotation);
	~Wall() override;
	/** Dummy (wall do nothing) */
	virtual void update(float deltatime) override;
};
