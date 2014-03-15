#ifndef CORPSE_H
#define CORPSE_H

#include "../src/Actor.h"

class Corpse : public Actor
{
public:
	Corpse(World *ownerWorld, Vector2D location, Rotator rotation);
	~Corpse(void);
	
	virtual void takeDamage(float damageValue, Vector2D impulse);
protected:
	virtual void updateCollision();
};

#endif