#ifndef BULLET_H
#define BULLET_H

#include "Globals.h"
#include "../src/World.h"
#include "../src/Actor.h"

class Bullet : public Actor
{
public:
	Bullet(World *ownerWorld, Vector2D location, Vector2D targetLocation);
	~Bullet(void);
	/** Process moving and other actions of the Actor */
	void Update(float deltaTime);
	/** Render the actor in the current location */
	void Render(Vector2D shift, Rotator angle);
	bool Destroyed;
protected:
	void UpdateCollision();
	Rotator Direction;
	World *OwnerWorld;
	float Speed;
	HTEXTURE BulletTexture;
};

#endif