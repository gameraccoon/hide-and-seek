#ifndef BULLET_H
#define BULLET_H

#include "Globals.h"
#include "../src/World.h"
#include "../src/Actor.h"

class Bullet : public Actor
{
public:
	Bullet(World *ownerWorld, Vector2D location);
	~Bullet(void);
	/** Process moving and other actions of the Actor */
	void Update(float deltaTime);
	/** Render the actor in the current location */
	void Render(Vector2D shift, Rotator angle);
private:
	World *OwnerWorld;
	float Speed;
	HTEXTURE BulletTexture;
};

#endif