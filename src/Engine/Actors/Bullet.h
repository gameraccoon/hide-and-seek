#ifndef BULLET_H
#define BULLET_H

#include "../Core/World.h"
#include "../Actors/Actor.h"
#include "../Modules/RayTrace.h"

class Bullet : public Actor
{
public:
	Bullet(World *ownerWorld, Vector2D location, Rotator rotation);
	~Bullet(void);
	/** Process moving and other actions of the Actor */
	void update(float deltatime);
	/** Take some damage to the bullet */
	virtual void takeDamage(float damageValue, Vector2D impulse);
protected:
	void updateCollision();
	World* ownerWorld;
	float speed;
};

#endif
