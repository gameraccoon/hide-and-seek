#ifndef TEST_WALL_H
#define TEST_WALL_H

#include <Engine/Actors/Actor.h>

class TestWall : public Actor
{
public:
	TestWall(World *world, Vector2D location, Vector2D scale, Rotator rotation);
	~TestWall(void);
	virtual void update(float deltatime) override final;
};

#endif
