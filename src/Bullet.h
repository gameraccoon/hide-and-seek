#ifndef BULLET_H
#define BULLET_H

#include "Globals.h"
#include "../src/World.h"
#include "../src/Actor.h"
#include "../src/RayTrace.h"

class Bullet : public Actor
{
public:
	Bullet(World *ownerWorld, Vector2D location, Vector2D targetLocation);
	~Bullet(void);
	/** Process moving and other actions of the Actor */
	void update(float deltatime);
	/** Render the actor in the current location */
	void render(Vector2D shift, Rotator angle);
	/** Take some damage to the bullet */
	virtual void takeDamage(float damageValue, Vector2D impulse);
protected:
	void updateCollision();
	Rotator direction;
	World* ownerWorld;
	float speed;
	HTEXTURE bulletTexture;
};

#endif
