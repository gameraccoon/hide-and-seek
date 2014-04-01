#ifndef WALL_H
#define WALL_H

#include "../../Engine/Actors/Actor.h"

class Wall : public Actor
{
public:
	Wall(World *world, Vector2D location, Vector2D scale, Rotator rotation);
	~Wall(void);
	/** Dummy (wall do nothing) */
	void update(float deltatime);
	/** Try to take some damage to the wall =) */
	virtual void takeDamage(float damageValue, Vector2D impulse);
private:
	/** */
	void updateCollision();
};

#endif
